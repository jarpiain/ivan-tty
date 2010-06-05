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

#ifndef __SAVE_H__
#define __SAVE_H__

#include <ctime>
#include <cstdio>
#include <vector>
#include <list>
#include <map>
#include <set>

#include "error.h"
#include "festring.h"
#include "fearray.h"

typedef std::map<festring, long> valuemap;

/* fstream seems to bug with DJGPP, so we use FILE* here */

class outputfile
{
 public:
  outputfile(const festring&, truth = true);
  ~outputfile();
  void Put(char What) { fputc(What, Buffer); }
  void Write(const char* Offset, long Size)
  { fwrite(Offset, 1, Size, Buffer); }
  truth IsOpen() { return truth(Buffer); }
  void Close() { fclose(Buffer); }
  void Flush() { fflush(Buffer); }
  void ReOpen();
 private:
  FILE* Buffer;
  festring FileName;
};

class inputfile
{
 public:
  inputfile(const festring&, const valuemap* = 0, truth = true);
  ~inputfile();
  festring ReadWord(truth = true);
  void ReadWord(festring&, truth = true);
  char ReadLetter(truth = true);
  long ReadNumber(int = 0xFF, truth = false);
  v2 ReadVector2d();
  rect ReadRect();
  int Get() { return fgetc(Buffer); }
  void Read(char* Offset, long Size) { fread(Offset, 1, Size, Buffer); }
  truth IsOpen() { return truth(Buffer); }
  truth Eof() { return feof(Buffer); }
  void ClearFlags() { clearerr(Buffer); }
  void SeekPosBegin(long Offset) { fseek(Buffer, Offset, SEEK_SET); }
  void SeekPosCurrent(long Offset) { fseek(Buffer, Offset, SEEK_CUR); }
  void SeekPosEnd(long Offset) { fseek(Buffer, Offset, SEEK_END); }
  long TellPos() { return ftell(Buffer); }
  ulong TellLine() { return TellLineOfPos(TellPos()); }
  ulong TellLineOfPos(long);
  const festring& GetFileName() const { return FileName; }
  void Close() { fclose(Buffer); }
 private:
  int HandlePunct(festring&, int, int);
  FILE* Buffer;
  festring FileName;
  const valuemap* ValueMap;
};

/* Reads a binary form variable of type type and returns it.
 * An inputfile template member function would be far more elegant,
 * but VC doesn't seem to understand it. */

template <class type> inline type ReadType(inputfile& SaveFile)
{
  type Variable;
  SaveFile >> Variable;
  return Variable;
}

inline void ReadData(char& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadNumber(); }
inline void ReadData(uchar& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadNumber(); }
inline void ReadData(short& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadNumber(); }
inline void ReadData(ushort& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadNumber(); }
inline void ReadData(long& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadNumber(); }
inline void ReadData(ulong& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadNumber(); }
inline void ReadData(int& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadNumber(); }
inline void ReadData(packv2& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadVector2d(); }
inline void ReadData(v2& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadVector2d(); }
inline void ReadData(rect& Type, inputfile& SaveFile)
{ Type = SaveFile.ReadRect(); }
void ReadData(festring&, inputfile&);
void ReadData(fearray<long>&, inputfile&);
void ReadData(fearray<festring>&, inputfile&);

template <class type>
inline void ReadData(fearray<type>& Array, inputfile& SaveFile)
{
  Array.Clear();
  festring Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    SaveFile.ReadWord(Word);

  if(Word == "=")
  {
    Array.Allocate(1);
    ReadData(Array.Data[0], SaveFile);
    return;
  }

  if(Word != "{")
    ABORT("Array syntax error \"%s\" found in file %s, line %ld!",
	  Word.CStr(), SaveFile.GetFileName().CStr(), SaveFile.TellLine());

  typedef typename fearray<type>::sizetype sizetype;
  sizetype Size = SaveFile.ReadNumber();
  Array.Allocate(Size);

  for(sizetype c = 0; c < Size; ++c)
    ReadData(Array.Data[c], SaveFile);

  if(SaveFile.ReadWord() != "}")
    ABORT("Illegal array terminator \"%s\" encountered in file %s, line %ld!",
	  Word.CStr(), SaveFile.GetFileName().CStr(), SaveFile.TellLine());
}

inline outputfile& operator<<(outputfile& SaveFile, char Value)
{
  SaveFile.Put(Value);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, char& Value)
{
  Value = SaveFile.Get();
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, uchar Value)
{
  SaveFile.Put(Value);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, uchar& Value)
{
  Value = SaveFile.Get();
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, short Value)
{
  SaveFile.Put(Value);
  SaveFile.Put(Value >> 8);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, short& Value)
{
  int LowWord = SaveFile.Get();
  Value = SaveFile.Get() << 8 | LowWord;
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ushort Value)
{
  SaveFile.Put(Value);
  SaveFile.Put(Value >> 8);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ushort& Value)
{
  int LowWord = SaveFile.Get();
  Value = SaveFile.Get() << 8 | LowWord;
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, long Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, long& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ulong Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ulong& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, unsigned Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, unsigned& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, int Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, int& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, double Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, double& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, packv2 Vector)
{
  SaveFile.Write(reinterpret_cast<char*>(&Vector), sizeof(Vector));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, packv2& Vector)
{
  SaveFile.Read(reinterpret_cast<char*>(&Vector), sizeof(Vector));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, v2 Vector)
{
  SaveFile.Write(reinterpret_cast<char*>(&Vector), sizeof(Vector));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, v2& Vector)
{
  SaveFile.Read(reinterpret_cast<char*>(&Vector), sizeof(Vector));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, const rect& Rect)
{
  SaveFile.Write(reinterpret_cast<const char*>(&Rect), sizeof(Rect));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, rect& Rect)
{
  SaveFile.Read(reinterpret_cast<char*>(&Rect), sizeof(Rect));
  return SaveFile;
}

outputfile& operator<<(outputfile&, const festring&);
inputfile& operator>>(inputfile&, festring&);
outputfile& operator<<(outputfile&, const char*);
inputfile& operator>>(inputfile&, char*&);

template <class type1, class type2>
inline outputfile& operator<<(outputfile& SaveFile,
			      const std::pair<type1, type2>& Pair)
{
  SaveFile << Pair.first << Pair.second;
  return SaveFile;
}

template <class type1, class type2>
inline inputfile& operator>>(inputfile& SaveFile,
			     std::pair<type1, type2>& Pair)
{
  SaveFile >> Pair.first >> Pair.second;
  return SaveFile;
}

template <class type>
inline outputfile& operator<<(outputfile& SaveFile,
			      const std::vector<type>& Vector)
{
  SaveFile << ulong(Vector.size());

  for(ulong c = 0; c < Vector.size(); ++c)
    SaveFile << Vector[c];

  return SaveFile;
}

template <class type>
inline inputfile& operator>>(inputfile& SaveFile,
			     std::vector<type>& Vector)
{
  Vector.resize(ReadType<ulong>(SaveFile), type());

  for(ulong c = 0; c < Vector.size(); ++c)
    SaveFile >> Vector[c];

  return SaveFile;
}

template <class type>
inline outputfile& operator<<(outputfile& SaveFile,
			      const std::list<type>& List)
{
  SaveFile << ulong(List.size());

  for(typename std::list<type>::const_iterator i = List.begin();
      i != List.end(); ++i)
    SaveFile << *i;

  return SaveFile;
}

template <class type>
inline inputfile& operator>>(inputfile& SaveFile,
			     std::list<type>& List)
{
  List.resize(ReadType<ulong>(SaveFile), type());

  for(typename std::list<type>::iterator i = List.begin();
      i != List.end(); ++i)
    SaveFile >> *i;

  return SaveFile;
}

template <class type1, class type2>
inline outputfile& operator<<(outputfile& SaveFile,
			      const std::map<type1, type2>& Map)
{
  SaveFile << ulong(Map.size());

  for(typename std::map<type1, type2>::const_iterator i = Map.begin();
      i != Map.end(); ++i)
    SaveFile << i->first << i->second;

  return SaveFile;
}

template <class type1, class type2>
inline inputfile& operator>>(inputfile& SaveFile,
			     std::map<type1, type2>& Map)
{
  Map.clear();
  ulong Size;
  SaveFile >> Size;

  for(ulong c = 0; c < Size; ++c)
  {
    type1 First;
    SaveFile >> First;
    SaveFile >> Map[First];
  }

  return SaveFile;
}

template <class type>
inline outputfile& operator<<(outputfile& SaveFile,
			      const std::set<type>& Set)
{
  SaveFile << ulong(Set.size());

  for(typename std::set<type>::const_iterator i = Set.begin();
      i != Set.end(); ++i)
    SaveFile << *i;

  return SaveFile;
}

template <class type>
inline inputfile& operator>>(inputfile& SaveFile,
			     std::set<type>& Set)
{
  Set.clear();
  ulong Size;
  SaveFile >> Size;

  for(ulong c = 0; c < Size; ++c)
  {
    type Value;
    SaveFile >> Value;
    Set.insert(Value);
  }

  return SaveFile;
}

template <class type>
inline outputfile& operator<<(outputfile& SaveFile,
			      const fearray<type>& Array)
{
  typename fearray<type>::sizetype c, Size = Array.Size;
  SaveFile << Size;

  for(c = 0; c < Size; ++c)
    SaveFile << Array[c];

  return SaveFile;
}

template <class type>
inline inputfile& operator>>(inputfile& SaveFile,
			     fearray<type>& Array)
{
  typename fearray<type>::sizetype c, Size;
  SaveFile >> Size;
  Array.Allocate(Size);

  for(c = 0; c < Size; ++c)
    SaveFile >> Array[c];

  return SaveFile;
}

template <class type>
inline outputfile& SaveLinkedList(outputfile& SaveFile,
				  const type* Element)
{
  for(const type* E = Element; E; E = E->Next)
    {
      SaveFile.Put(true);
      SaveFile << E;
    }

  SaveFile.Put(false);
  return SaveFile;
}

template <class type>
inline inputfile& LoadLinkedList(inputfile& SaveFile,
				 type*& Element)
{
  if(SaveFile.Get())
  {
    SaveFile >> Element;
    type* E;

    for(E = Element; SaveFile.Get(); E = E->Next)
      SaveFile >> E->Next;

    E->Next = 0;
  }
  else
    Element = 0;

  return SaveFile;
}

template <class type>
inline outputfile& SaveArray(outputfile& SaveFile,
			     const type* Array, int Count)
{
  for(int c = 0; c < Count; ++c)
    SaveFile << Array[c];

  return SaveFile;
}

template <class type>
inline inputfile& LoadArray(inputfile& SaveFile,
			    type* Array, int Count)
{
  for(int c = 0; c < Count; ++c)
    SaveFile >> Array[c];

  return SaveFile;
}

#endif
