#include "commands.hpp"
#include "vector.hpp"
#include <iostream>
#include <string>

namespace {

  bool isSpace(char c)
  {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
  }

  pozdeev::Vector< std::string > splitWords(const std::string & line)
  {
    pozdeev::Vector< std::string > words;
    size_t i = 0;
    while (i < line.length()) {
      while (i < line.length() && isSpace(line[i])) {
        i++;
      }
      if (i < line.length()) {
        size_t start = i;
        while (i < line.length() && !isSpace(line[i])) {
          i++;
        }
        words.pushBack(line.substr(start, i - start));
      }
    }
    return words;
  }

  int stringToInt(const std::string & str)
  {
    int result = 0;
    bool negative = false;
    size_t i = 0;
    if (str[i] == '-') {
      negative = true;
      i++;
    }
    for (; i < str.length(); ++i) {
      result = result * 10 + (str[i] - '0');
    }
    return negative ? -result : result;
  }

}

namespace pozdeev {

  void parseFile(std::istream & in, DataSets & dataSets)
  {
    std::string line;
    while (std::getline(in, line)) {
      Vector< std::string > words = splitWords(line);
      if (words.size() == 0) {
        continue;
      }

      std::string dsName = words[0];
      Dictionary dict;

      for (size_t i = 1; i + 1 < words.size(); i += 2) {
        int key = stringToInt(words[i]);
        std::string value = words[i + 1];
        dict.push(key, value);
      }

      dataSets.push(dsName, dict);
    }
  }

  void executePrint(const std::string & dsName, DataSets & dataSets, std::ostream & out)
  {
    if (!dataSets.has(dsName)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary & dict = dataSets.get(dsName);
    if (dict.empty()) {
      out << "<EMPTY>\n";
      return;
    }

    out << dsName;
    for (auto it = dict.cbegin(); it != dict.cend(); ++it) {
      out << " " << it->first << " " << it->second;
    }
    out << "\n";
  }

  void executeComplement(const std::string & newDs, const std::string & ds1, const std::string & ds2, DataSets & dataSets, std::ostream & out)
  {
    if (!dataSets.has(ds1) || !dataSets.has(ds2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary & dict1 = dataSets.get(ds1);
    const Dictionary & dict2 = dataSets.get(ds2);
    Dictionary result;

    for (auto it = dict1.cbegin(); it != dict1.cend(); ++it) {
      if (!dict2.has(it->first)) {
        result.push(it->first, it->second);
      }
    }

    dataSets.push(newDs, result);
  }

  void executeIntersect(const std::string & newDs, const std::string & ds1, const std::string & ds2, DataSets & dataSets, std::ostream & out)
  {
    if (!dataSets.has(ds1) || !dataSets.has(ds2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary & dict1 = dataSets.get(ds1);
    const Dictionary & dict2 = dataSets.get(ds2);
    Dictionary result;

    for (auto it = dict1.cbegin(); it != dict1.cend(); ++it) {
      if (dict2.has(it->first)) {
        result.push(it->first, it->second);
      }
    }

    dataSets.push(newDs, result);
  }

  void executeUnion(const std::string & newDs, const std::string & ds1, const std::string & ds2, DataSets & dataSets, std::ostream & out)
  {
    if (!dataSets.has(ds1) || !dataSets.has(ds2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary & dict1 = dataSets.get(ds1);
    const Dictionary & dict2 = dataSets.get(ds2);
    Dictionary result;

    for (auto it = dict1.cbegin(); it != dict1.cend(); ++it) {
      result.push(it->first, it->second);
    }

    for (auto it = dict2.cbegin(); it != dict2.cend(); ++it) {
      if (!result.has(it->first)) {
        result.push(it->first, it->second);
      }
    }

    dataSets.push(newDs, result);
  }

  void processCommands(std::istream & in, std::ostream & out, DataSets & dataSets)
  {
    std::string line;
    while (std::getline(in, line)) {
      Vector< std::string > args = splitWords(line);
      if (args.size() == 0) {
        continue;
      }

      const std::string & cmd = args[0];
      if (cmd == "print") {
        if (args.size() != 2) {
          out << "<INVALID COMMAND>\n";
        } else {
          executePrint(args[1], dataSets, out);
        }
      } else if (cmd == "complement") {
        if (args.size() != 4) {
          out << "<INVALID COMMAND>\n";
        } else {
          executeComplement(args[1], args[2], args[3], dataSets, out);
        }
      } else if (cmd == "intersect") {
        if (args.size() != 4) {
          out << "<INVALID COMMAND>\n";
        } else {
          executeIntersect(args[1], args[2], args[3], dataSets, out);
        }
      } else if (cmd == "union") {
        if (args.size() != 4) {
          out << "<INVALID COMMAND>\n";
        } else {
          executeUnion(args[1], args[2], args[3], dataSets, out);
        }
      } else {
        out << "<INVALID COMMAND>\n";
      }
    }
  }

}
