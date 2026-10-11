module;

#include <vector>      // std::vector
#include <tuple>       // std::tuple
#include <array>       // std::array
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
  [[nodiscard]] static consteval std::size_t GetIndex() noexcept
  {
    static_assert((std::is_same_v<T, Types> || ...), "Type T is not in Types pack");
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

  template <typename T>
  [[nodiscard]] constexpr bool HasComponent(EntityId_t const eId) const noexcept
  {
    return (bitMaskComponents_[eId] & GetMask<T>()) != 0;
  }

  template <class Cmp>
  [[nodiscard]] constexpr Cmp& GetComponent(this auto&& self, EntityId_t const eId) noexcept
    pre(self.template HasComponent<Cmp>(eId))
  {
    auto const& vecIdxs = self.entityComponentIndex_[GetIndex<Cmp>()];
    auto const entityCmpIdx = vecIdxs.at(eId);
    return self.data_.template GetComponents<Cmp>().at(entityCmpIdx);
  }

  template <typename K>
  [[nodiscard]] constexpr auto& GetComponents(this auto&& self) noexcept
  {
    return std::get<vector_t<K>>(self.data_);
  }

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
};
} // namespace TinyEngine
