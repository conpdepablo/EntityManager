module;

#include <vector>  // std::vector
#include <tuple>   // std::tuple
#include <cstdint> // std::uint32_t

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

private:
  Storage_t data_{ };
};
} // namespace TinyEngine
