#include <cassert>
#include <print>

import TinyEngine.EntityManager;

using TinyEngine::EntityManager;

auto CreateEntityManager() { return EntityManager<float, int, char, double>{ }; }

void printBitMask()
{
  auto em = CreateEntityManager();

  std::println("Printing BitMask for: EntityManager<float, int, char, double>");
  std::println(" Mask<float>: {}", em.template GetMask<float>());
  std::println("   Mask<int>: {}", em.template GetMask<int>());
  std::println("  Mask<char>: {}", em.template GetMask<char>());
  std::println("Mask<double>: {}", em.template GetMask<double>());
}

void Test_GetIndex()
{
  auto em = CreateEntityManager();

  static_assert(em.template GetIndex<float>() == 0);
  static_assert(em.template GetIndex<int>() == 1);
  static_assert(em.template GetIndex<char>() == 2);
  static_assert(em.template GetIndex<double>() == 3);
}

void Test_GetMask()
{
  auto em = CreateEntityManager();

  static_assert(em.template GetMask<float>() == 0b0001);
  static_assert(em.template GetMask<int>() == 0b0010);
  static_assert(em.template GetMask<char>() == 0b0100);
  static_assert(em.template GetMask<double>() == 0b1000);
}

void Test_CreateEntity()
{
  auto em = CreateEntityManager();

  auto const eId = em.CreateEntity(10.f, 'a');

  assert(em.template HasComponent<float>(eId));
  assert(!em.template HasComponent<int>(eId));
  assert(em.template HasComponent<char>(eId));
  assert(!em.template HasComponent<double>(eId));
}

void Test_CreateMultipleEntities()
{
  auto em = CreateEntityManager();

  auto const eId_1 = em.CreateEntity(10.f, 'a');
  assert(em.template HasComponent<float>(eId_1));
  assert(em.template HasComponent<char>(eId_1));

  auto const eId_2 = em.CreateEntity(40, 40.0);
  assert(em.template HasComponent<int>(eId_2));
  assert(em.template HasComponent<double>(eId_2));
}

void Test_CreateClearCreate()
{
  auto em = CreateEntityManager();

  [[maybe_unused]] auto const eId_1 = em.CreateEntity(4);
  em.Clear();
  [[maybe_unused]] auto const eId_2 = em.CreateEntity(5.0);

  assert(em.GetEntityNumber() == 1);
}

void Test_RemoveLastEntity()
{
  auto em = CreateEntityManager();
  auto const eId = em.CreateEntity('e');
  em.RemoveEntity(eId);

  assert(em.GetEntityNumber() == 0);
}

void Test_RemoveEntity()
{
  auto em = CreateEntityManager();
  auto const eId_1 = em.CreateEntity(4);
  [[maybe_unused]] auto const eId_2 = em.CreateEntity(5.0);
  [[maybe_unused]] auto const eId_3 = em.CreateEntity(6, 7.0);

  em.RemoveEntity(eId_1);
  assert(em.GetEntityNumber() == 2);
}

int main()
{
  printBitMask();
  Test_GetIndex();
  Test_GetMask();
  Test_CreateEntity();
  Test_CreateMultipleEntities();
  Test_RemoveEntity();
}
