#include <cassert>

import TinyEngine.EntityManager;

using TinyEngine::EntityManager;

void Test_GetIndex()
{
  EntityManager<float, int, char, double> em{ };

  static_assert(em.template GetIndex<float>() == 0);
  static_assert(em.template GetIndex<int>() == 1);
  static_assert(em.template GetIndex<char>() == 2);
  static_assert(em.template GetIndex<double>() == 3);
}

void Test_GetMask()
{
  EntityManager<float, int, char, double> em{ };

  static_assert(em.template GetMask<float>() == 0b0001);
  static_assert(em.template GetMask<int>() == 0b0010);
  static_assert(em.template GetMask<char>() == 0b0100);
  static_assert(em.template GetMask<double>() == 0b1000);
}

void Test_CreateEntity()
{
  EntityManager<float, int, char, double> em{ };

  auto const eId = em.template CreateEntity(10.f, 20, 'a');

  assert(em.template HasComponent<float>(eId));
  assert(em.template HasComponent<int>(eId));
  assert(em.template HasComponent<char>(eId));
  assert(!em.template HasComponent<double>(eId));
}

int main()
{
  Test_GetIndex();
  Test_GetMask();
}
