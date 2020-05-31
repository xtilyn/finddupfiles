// skeleton program for find duplicate files using sha256 digests
//
// - it reads filenames form standard input
// - it then computes sha256 digests for first two files
// - compares the digests
//
// the program uses C++ containers and strings, but otherwise
// uses standard C APIs
// ===================================================================

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

// read line from fp
// returns False on EOF
//         True  on success
bool get_line(std::string &line, FILE *fp = stdin)
{
  line.clear();
  while (1)
  {
    int c = fgetc(fp);
    if (c == '\n')
      return true;
    if (c == EOF)
      return !line.empty();
    line.push_back(c);
  }
}

// returns SHA256 digest of the contents of file fname
// on failure returns empty string
// uses popen() to call sha256sum binary
std::string
digest(const std::string &fname)
{
  std::string cmdline = "sha256sum " + fname + " 2> /dev/null";
  FILE *fp = popen(cmdline.c_str(), "r");
  if (fp == NULL)
    return "";
  std::string output;
  if (!get_line(output, fp))
    return "";
  if (pclose(fp) != 0)
    return "";
  std::string result;
  for (auto c : output)
    if (isspace(c))
      break;
    else
      result.push_back(c);
  return result;
}

int main()
{
  // read a list of filenames from stdin
  std::vector<std::string> fnames;
  std::string line;
  while (1)
  {
    if (!get_line(line))
      break;
    fnames.push_back(line);
  }

  if (fnames.size() < 2)
  {
    printf("I could have worked if you gave me 2+ filenames... :(\n");
    return -1;
  }

  std::vector<std::string> invalid;
  std::unordered_map<std::string, std::vector<std::string>> digestsMap;
  for (int i = 0; i < fnames.size(); i++)
  {
    std::string dig = digest(fnames[i]);
    if (dig.empty())
    {
      invalid.push_back(fnames[i].c_str());
    }
    else
    {
      if (digestsMap.find(dig) == digestsMap.end())
      {
        digestsMap[dig] = std::vector<std::string>();
      }

      digestsMap[dig].push_back(fnames[i].c_str());
    }
  }

  int num = 1;
  for (auto it : digestsMap)
  {
    if (it.second.size() > 1)
    {
      printf("Match %d\n", num);
      for (int j = 0; j < it.second.size(); j++)
      {
        printf("    - %s\n", it.second[j].c_str());
      }
      num++;
    }
  }

  if (invalid.size() > 0)
    printf("Could not compute digests for files:\n");
  for (int k = 0; k < invalid.size(); k++)
  {
    printf("    - %s\n", invalid[k].c_str());
  }

  return 0;
}
