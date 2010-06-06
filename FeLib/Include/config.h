/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <fstream>

#include "festring.h"

class inputfile;
class felist;
struct configoption;
struct stringoption;
struct numberoption;
struct truthoption;

class configsystem
{
 public:
  static truth Load();
  static void AddOption(configoption*);
  static void SetConfigFileName(const festring& What)
  { ConfigFileName = What; }
 private:
  static configoption* Option[MAX_CONFIG_OPTIONS];
  static festring ConfigFileName;
  static int Options;
};

/* Currently there's no human-readable output option in outputfile,
   so we're forced to use std::ofstream */

struct configoption
{
  configoption(const char*, const char*);
  virtual ~configoption() { }
  virtual void LoadValue(inputfile&) = 0;
  const char* Name;
  const char* Description;
};

struct stringoption : public configoption
{
  stringoption(const char*, const char*, const festring&);
  virtual void LoadValue(inputfile&);
  festring Value;
};

struct numberoption : public configoption
{
  numberoption(const char*, const char*, long);
  virtual void LoadValue(inputfile&);
  long Value;
};

struct scrollbaroption : public numberoption
{
  scrollbaroption(const char*, const char*, long);
};

struct truthoption : public configoption
{
  truthoption(const char*, const char*, truth);
  virtual void LoadValue(inputfile&);
  truth Value;
};

#endif
