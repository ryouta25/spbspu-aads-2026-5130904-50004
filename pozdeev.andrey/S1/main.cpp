#include "biList.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <limits>

int main()
{
  pozdeev::BiList< std::pair< std::string, pozdeev::BiList< unsigned long long > > > sequences;
  std::string name;

  if (!(std::cin >> name))
  {
    std::cout << "0\n";
    return 0;
  }

  pozdeev::BiList< unsigned long long > numbers;

  while (!std::cin.eof())
  {
    unsigned long long num = 0;

    if (std::cin >> num)
    {
      numbers.pushBack(num);
    }
    else
    {
      if (std::cin.eof())
      {
        break;
      }

      std::cin.clear();

      sequences.pushBack(std::make_pair(name, std::move(numbers)));
      numbers.clear();

      std::cin >> name;
    }
  }

  sequences.pushBack(std::make_pair(name, std::move(numbers)));

  if (sequences.isEmpty())
  {
    std::cout << "0\n";
    return 0;
  }

  size_t maxLen = 0;
  bool allEmpty = true;

  auto it = sequences.cbegin();

  std::cout << it->first;

  if (it->second.getSize() > maxLen)
  {
    maxLen = it->second.getSize();
  }
  if (!it->second.isEmpty())
  {
    allEmpty = false;
  }
  ++it;

  for (; it != sequences.cend(); ++it)
  {
    std::cout << " " << it->first;

    if (it->second.getSize() > maxLen)
    {
      maxLen = it->second.getSize();
    }
    if (!it->second.isEmpty())
    {
      allEmpty = false;
    }
  }
  std::cout << "\n";

  if (allEmpty)
  {
    std::cout << "0\n";
    return 0;
  }

  pozdeev::BiList< unsigned long long > resultSums;

  for (size_t i = 0; i < maxLen; ++i)
  {
    unsigned long long currentTotal = 0;
    bool isFirstInRow = true;

    for (auto seqIt = sequences.cbegin(); seqIt != sequences.cend(); ++seqIt)
    {
      if (i < seqIt->second.getSize())
      {
        auto elemIt = seqIt->second.cbegin();
        for (size_t j = 0; j < i; ++j)
        {
          ++elemIt;
        }

        unsigned long long val = *elemIt;

        if (isFirstInRow)
        {
          std::cout << val;
          isFirstInRow = false;
        }
        else
        {
          std::cout << " " << val;
        }

        if (currentTotal > std::numeric_limits< unsigned long long >::max() - val)
        {
          std::cout << "\n";
          std::cerr << "Overflow\n";
          return 1;
        }
        currentTotal += val;
      }
    }
    std::cout << "\n";
    resultSums.pushBack(currentTotal);
  }

  if (!resultSums.isEmpty())
  {
    auto sumIt = resultSums.cbegin();

    std::cout << *sumIt;
    ++sumIt;

    for (; sumIt != resultSums.cend(); ++sumIt)
    {
      std::cout << " " << *sumIt;
    }
    std::cout << "\n";
  }

  return 0;
}
