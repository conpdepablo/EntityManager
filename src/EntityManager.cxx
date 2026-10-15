module;

#include <print>
#include <vector>      // std::vector
#include <tuple>       // std::tuple
#include <array>       // std::array
#include <algorithm>   // std::ranges::
#include <cstdint>     // std::uint32_t
#include <type_traits> // std::is_same_v, sts::remove_cvref

export module TinyEngine.EntityManager;

export namespace TinyEngine
{
template <typename... Types>
struct [[nodiscard]] EntityManager
{
  template <typename T>
  using vector_t = std::vector<T>;
  using Storage_t = std::tuple<vector_t<Types>...>;

  using Mask_t = std::uint32_t; // TODO: Check Cmps size.
  using BitMask_t = std::vector<Mask_t>;

  using EntityId_t = std::size_t;
  using EntityComponentIndex_t = std::array<std::vector<EntityId_t>, sizeof...(Types)>;

  template <typename T>
  [[nodiscard]] static consteval std::size_t GetIndex() noexcept pre(typeIsPresent<T>())
  {
    constexpr std::array<bool, sizeof...(Types)> matches{ std::is_same_v<T, Types>... };
    for (std::size_t i = 0; i < matches.size(); ++i)
    {
      if (matches[i])
      {
        return static_cast<Mask_t>(i);
      }
    }
  }

  template <typename T>
  [[nodiscard]] static consteval Mask_t GetMask() noexcept
  {
    return Mask_t(1) << GetIndex<T>();
  }

  template <typename... Ts>
  [[nodiscard]] constexpr EntityId_t CreateEntity(Ts&&... components) noexcept
  {
    EntityId_t const eId = bitMaskComponents_.size();
    bitMaskComponents_.emplace_back();

    for (auto& component : entityComponentIndex_)
    {
      component.emplace_back(); // Reserve space for all components.
      // TODO: Here we chan check which compoents we have, and just get space for those.
    }

    (
      [&]
      {
        using T = std::remove_cvref_t<Ts>;
        bitMaskComponents_[eId] |= GetMask<T>(); // Enable component in bitmask.

        auto const cmpIdx = Add<T>(std::forward<Ts>(components)); // Store component
        entityComponentIndex_[GetIndex<T>()][eId] = cmpIdx;       // Map entity -> component index.
      }(),
      ...);

    return eId;
  }

  constexpr void RemoveEntity(EntityId_t const eId) noexcept pre(canEntityBePresent(eId))
  {
    auto const lastEntity = bitMaskComponents_.size() - 1;

    // Remove the components owned by eId and update the mappings.
    (
      [&]
      {
        using T = Types;

        if (HasComponent<T>(eId))
        {
          auto& components = GetComponents<T>();
          auto& indices = entityComponentIndex_[GetIndex<T>()];

          auto const cmpIdx = indices[eId];
          auto const lastCmpIdx = components.size() - 1;

          if (cmpIdx != lastCmpIdx)
          {
            // Find the entity that owns the last component.
            EntityId_t lastOwner = 0;

            for (EntityId_t i = 0; i < bitMaskComponents_.size(); ++i)
            {
              if (i != eId && HasComponent<T>(i) && indices[i] == lastCmpIdx)
              {
                lastOwner = i;
                break;
              }
            }

            std::swap(components[cmpIdx], components[lastCmpIdx]);
            indices[lastOwner] = cmpIdx;
          }

          components.pop_back();
        }

        // The last entity will move into eId's position.
        if (eId != lastEntity && HasComponent<T>(lastEntity))
        {
          entityComponentIndex_[GetIndex<T>()][eId] = entityComponentIndex_[GetIndex<T>()][lastEntity];
        }
      }(),
      ...);

    // Remove the entity only once, after processing every component type.
    if (eId != lastEntity)
    {
      bitMaskComponents_[eId] = bitMaskComponents_[lastEntity];
    }

    bitMaskComponents_.pop_back();
    std::ranges::for_each(entityComponentIndex_, [](auto& components) { components.pop_back(); });
  }

  template <typename T>
  [[nodiscard]] constexpr bool HasComponent(EntityId_t const eId) const noexcept pre(typeIsPresent<T>())
  {
    return (bitMaskComponents_[eId] & GetMask<T>()) != 0;
  }

  template <typename T>
  [[nodiscard]] constexpr T& GetComponent(this auto&& self, EntityId_t const eId) noexcept
    pre(typeIsPresent<T>() && self.template HasComponent<T>(eId))
  {
    auto const& vecIdxs = self.entityComponentIndex_[GetIndex<T>()];
    auto const entityCmpIdx = vecIdxs.at(eId);
    return self.data_.template GetComponents<T>().at(entityCmpIdx);
  }

  template <typename K>
  [[nodiscard]] constexpr auto& GetComponents(this auto&& self) noexcept
  {
    return std::get<vector_t<K>>(self.data_);
  }

  constexpr void Clear() noexcept
  {
    bitMaskComponents_.clear();
    std::ranges::for_each(entityComponentIndex_, [](auto& components) { components.clear(); });
    std::apply([](auto&... components) { (components.clear(), ...); }, data_);
  }

  [[nodiscard]] constexpr std::size_t GetEntityNumber() const noexcept { return bitMaskComponents_.size(); }

private:
  BitMask_t bitMaskComponents_{ };
  EntityComponentIndex_t entityComponentIndex_{ };
  Storage_t data_{ };

  template <typename T, typename... Ks>
  constexpr std::size_t Add(Ks&&... ks) noexcept
  {
    auto& vec = GetComponents<T>();
    vec.emplace_back(std::forward<Ks>(ks)...);
    return vec.size() - 1;
  }

  consteval bool canEntityBePresent(EntityId_t const eId) { return eId < bitMaskComponents_.size(); }
  template <typename T>
  static consteval bool typeIsPresent()
  {
    return (std::is_same_v<T, Types> || ...);
  }
};
} // namespace TinyEngine
