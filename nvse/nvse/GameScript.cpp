#include "GameAPI.h"
#include "GameScript.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "CommandTable.h"
#include "GameRTTI.h"
#include "internal/utility.h"


#if RUNTIME

void Script::RefVariable::Resolve(ScriptEventList* eventList) {
	if (varIdx && eventList) {
		ScriptVar* var = eventList->GetVariable(varIdx);
		if (var) form = LookupFormByID(*(UInt32*)&var->data);
	}
}

ScriptEventList* Script::CreateEventList(void) {
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
	return ThisCall<ScriptEventList*>(0x005ABF60, this);	// 4th sub above Script::Execute (was 1st above in Oblivion) Execute is the second to last call in Run
#else
#error
#endif
}

void Script::SetVarByName(ScriptEventList* eventList, const char* varName, float value)
{
	ListNode<VariableInfo>* traverse = varList.Head();
	VariableInfo* varInfo;
	do {
		varInfo = traverse->data;
		if (varInfo) {
			if (!strcmp(((char*)varInfo->name.CStr()), varName)) {
				ScriptVar* scv = eventList->GetVariable(varInfo->idx);
				if (scv) {
					scv->data = value;
					break;
				}
			}
		}
	} while (traverse = traverse->next);
}

#else		// CS-stuff below

#endif

UInt32 ScriptBuffer::GetRefIdx(Script::RefVariable* refVar) {
	return refVars.GetIndex(refVar);
}

/******************************
 Script
******************************/

class ScriptVarFinder {
	const char* m_varName;

public:
	ScriptVarFinder(const char* varName) : m_varName(varName) {}

	bool Accept(VariableInfo* varInfo) {
		return StrEqualCI(varInfo->name.m_data, m_varName);
	}
};

VariableInfo* Script::GetVariableByName(const char* varName) {
	ListNode<VariableInfo>* varIter = varList.Head();
	VariableInfo* varInfo;
	do {
		varInfo = varIter->data;
		if (varInfo && StrEqualCI(varName, varInfo->name.m_data))
			return varInfo;
	} while (varIter = varIter->next);
	return NULL;
}

Script::RefVariable* Script::GetVariable(UInt32 reqIdx) {
	UInt32 idx = 1;	// yes, really starts at 1
	if (reqIdx) {
		ListNode<RefVariable>* varIter = refList.Head();
		do {
			if (idx == reqIdx)
				return varIter->data;
			idx++;
		} while (varIter = varIter->next);
	}
	return NULL;
}

VariableInfo* Script::GetVariableInfo(UInt32 idx) {
	ListNode<VariableInfo>* varIter = varList.Head();
	VariableInfo* varInfo;
	do {
		varInfo = varIter->data;
		if (varInfo && (varInfo->idx == idx))
			return varInfo;
	} while (varIter = varIter->next);
	return NULL;
}

UInt32 Script::AddVariable(TESForm* form) {
	RefVariable* refVar = (RefVariable*)GameHeapAlloc(sizeof(RefVariable));
	refVar->name.Set("");
	refVar->form = form;
	refVar->varIdx = 0;

	UInt32 resultIdx = refList.Append(refVar) + 1;
	info.numRefs = resultIdx + 1;
	return resultIdx;
}

void Script::CleanupVariables() {
	refList.RemoveAll();
}

UInt32 Script::RefVarList::GetIndex(RefVariable* refVar) {
	UInt32 idx = 0;
	ListNode<RefVariable>* varIter = Head();
	do {
		idx++;
		if (varIter->data == refVar)
			return idx;
	} while (varIter = varIter->next);
	return 0;
}

/***********************************
 ScriptLineBuffer
***********************************/

//const char	* ScriptLineBuffer::kDelims_Whitespace = " \t\n\r";
//const char  * ScriptLineBuffer::kDelims_WhitespaceAndBrackets = " \t\n\r[]";

bool ScriptLineBuffer::Write(const void* buf, UInt32 bufsize) {
	if ((dataOffset + bufsize) >= kBufferSize) return false;
	memcpy(dataBuf + dataOffset, buf, bufsize);
	dataOffset += bufsize;
	return true;
}

bool ScriptLineBuffer::Write32(UInt32 buf) {
	if ((dataOffset + 4) >= kBufferSize) return false;
	*(UInt32*)(dataBuf + dataOffset) = buf;
	dataOffset += 4;
	return true;
}

bool ScriptLineBuffer::WriteString(const char* buf) {
	UInt32 len = StrLen(buf);
	if ((dataOffset + 2 + len) >= kBufferSize) return false;
	UInt8* dataPtr = dataBuf + dataOffset;
	*(UInt16*)dataPtr = len;
	memcpy(dataPtr + 2, buf, len);
	dataOffset += 2 + len;
	return true;
}

bool ScriptLineBuffer::Write16(UInt16 buf) {
	if ((dataOffset + 2) >= kBufferSize) return false;
	*(UInt16*)(dataBuf + dataOffset) = buf;
	dataOffset += 2;
	return true;
}

bool ScriptLineBuffer::WriteByte(UInt8 buf) {
	if ((dataOffset + 1) >= kBufferSize) return false;
	*(dataBuf + dataOffset) = buf;
	dataOffset++;
	return true;
}

bool ScriptLineBuffer::WriteFloat(double buf) {
	if ((dataOffset + 8) >= kBufferSize) return false;
	memcpy(dataBuf + dataOffset, &buf, 8);
	dataOffset += 8;
	return true;
}