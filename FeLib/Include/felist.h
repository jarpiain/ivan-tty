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

#ifndef __LIST_H__
#define __LIST_H__

#include <vector>

#include "v2.h"

class outputfile;
class inputfile;
class festring;
struct felistentry;
struct felistdescription;

class felist
{
 public:
  felist(const festring&, col16 = WHITE, uint = 0,
	 uint Row = 0, uint Col = 0, uint Width = 75, uint Height = 18);
  ~felist();
  void AddEntry(const festring&, col16, uint = 0,
		truth = true);
  void AddDescription(const festring&, col16 = WHITE);
  uint Draw();
  void QuickDraw(uint, uint, uint, uint) const;
  void Empty();
  void EmptyDescription();
  festring GetEntry(uint) const;
  col16 GetColor(uint) const;
  void SetColor(uint, col16);
  uint GetLength() const;
  uint GetLastEntryIndex() const;
  void Load(inputfile&);
  void Save(outputfile&) const;
  truth IsEmpty() const;
  uint GetSelected() const { return Selected; }
  void SetSelected(uint What) { Selected = What; }
  void EditSelected(int What) { Selected += What; }
  truth DrawPage() const;
  void Pop();
  void PrintToFile(const festring&);
  void SetPos(v2 What) { Pos = What; }
  void SetWidth(uint What) { Width = What; }
  void SetPageLength(uint What) { PageLength = What; }
  void SetBackColor(col16 What) { BackColor = What; }
  void SetFlags(uint What) { Flags = What; }
  void AddFlags(uint What) { Flags |= What; }
  void RemoveFlags(uint What) { Flags &= ~What; }
  void SetUpKey(uint What) { UpKey = What; }
  void SetDownKey(uint What) { DownKey = What; }
  static truth NeedRedraw();
 private:
  void DrawDescription() const;
  std::vector<felistentry*> Entry;
  std::vector<felistdescription*> Description;
  uint PageBegin;
  uint Maximum;
  uint Selected;
  v2 Pos;
  uint Width;
  uint PageLength;
  col16 BackColor;
  uint Flags;
  uint UpKey;
  uint DownKey;
};

#endif
