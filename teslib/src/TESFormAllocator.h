#pragma once

#include "TES.h"
#include "TESForm.h"

class TESFile;
class TESForm;
struct TESRecord;

class TESFormAllocator
{
public:
	TESFormAllocator();

	static void init();

	static TESForm* alloc( TESFile* _file, TESRecord* _record );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};


extern TESFormAllocator tesFormAllocator;

