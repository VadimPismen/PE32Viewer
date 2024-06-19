#pragma once

#define IDM_OPENFILE									2
#define IDS_OPENFILE									"Open file"

#define IDC_CLICKCODEBUTTON								200
#define IDC_FORWARDBUTTON								201
#define IDC_BACKBUTTON									202
#define IDC_SWITCHBUTTON								203

#define nRowsOnPage										64

#define S_MZ											"MZ"
const std::string S_PE = std::string("PE\0\0", 4);

#define S_NOTAPEFILE									"This is not a PE file!"
#define S_WARNING										"Warning"

#define S_DOSHEADER										"DOS header:"
#define S_DOSE_MAGIC									"e_magic = "
#define S_DOSE_LFANEW									"e_lfanew = "

#define S_DOSSTUB										"DOS stub:"

#define S_PEHEADER										"PE header:"
#define S_PESIGNATURE									"Signature = "

#define S_PEFILEHEADER									"File header:"
#define S_PEFILEHEADERMACHINE							"Machine = "
#define S_PEFILEHEADERNUMBEROFSECTIONS					"NumberOfSections = "
#define S_PEFILEHEADERTIMEDATESTAMP						"TimeDateStamp = "
#define S_PEFILEHEADERTIMEDATESTAMPBIGENDIAN			"TimeDateStamp (Big-endian) = "
#define S_PEFILEHEADERPOINTERTOSYMBOLTABLE				"PointerToSymbolTable = "
#define S_PEFILEHEADERNUMBEROFSYMBOLS					"NumberOfSymbols = "
#define S_PEFILEHEADERSIZEOFOPTIONALHEADER 				"SizeOfOptionalHeader = "
#define S_PEFILEHEADERCHARACTERISTICS 					"Characteristics = "

#define S_OPTIONALHEADER 								"Optional header :"
#define S_OPTIONALHEADERMAGIC							"Magic = "
#define S_PE32											" (PE32)"
#define S_PE32PLUS										" (PE32+)"
#define S_ROM											"(ROM)"
#define S_UNKNOWN										" (UNKNOWN)"
#define S_NOTSUPPORTED									" (READING IS NOT SUPPORTED)"
#define S_OPTIONALHEADERADDRESSOFENTRYPOINT				"AddressOfEntryPoint = "
#define S_OPTIONALHEADERBASEOFCODE						"BaseOfCode = "
#define S_OPTIONALHEADERBASEOFDATA						"BaseOfData = "
#define S_OPTIONALHEADERIMAGEBASE						"ImageBase = "
#define S_NOTAMULTIPLE									"(NOT A MULTIPLE OF "
#define S_OF64KIB										"64 KiB)"
#define S_B												" bytes"
#define S_OFSECTIONALIGNMENT							"SectionAlignment)"
#define S_OFFILEALIGNMENT								"FileAlignment)"
#define S_OPTIONALHEADERSECTIONALIGMENT					"SectionAligment = "
#define S_OPTIONALHEADERFILEALIGNMENT					"FileAlignment = "
#define S_OPTIONALHEADERMAJOROPERATINGSYSTEMVERSION		"MajorOperatingSystemVersion = "
#define S_OPTIONALHEADERMINOROPERATINGSYSTEMVERSION		"MinorOperatingSystemVersion = "
#define S_OPTIONALHEADERSIZEOFIMAGE						"SizeOfImage = "
#define S_OPTIONALHEADERSIZEOFHEADERS					"SizeOfHeaders = "
#define S_OPTIONALHEADERSUBSYSTEM						"Subsystem = "
#define S_OPTIONALHEADERNUMBEROFRVAANDSIZES				"NumberOfRvaAndSizes = "

#define S_OPTIONALHEADERDATADIRECTORY					"DataDirectory:"

const std::string S_OPTIONALHEADERDATADIRECTORYSECTIONNAMES[16] =
{ "Export Table", "Import Table", "Resource Table", "Exception Table",
"Certificate Table", "Base Relocation Table", "Debug", "Architecture",
"Global Ptr", "TLS Table", "Load Config Table", "Bound Import",
"IAT", "Delay Import Descriptor", "CLR Runtime Header", "Reserved" };


#define S_OPTIONALHEADERDATADIRECTORYVIRTUALADDRESS		"VirtualAddress = "
#define S_OPTIONALHEADERDATADIRECTORYSIZE				"Size = "
#define S_SECTIONHEADER									"Section header:"
#define S_SECTIONHEADERNAME								"Name = "
#define S_SECTIONHEADERVIRTUALSIZE						"VirtualSize = "
#define S_SECTIONHEADERVIRTUALADDRESS					"VirtualAddress = "
#define S_SECTIONHEADERSIZEOFRAWDATA					"SizeOfRawData = "
#define S_SECTIONHEADERPOINTERTORAWDATA					"PointerToRawData = "
#define S_SECTIONHEADERPOINTERTORELOCATIONS				"PointerToRelocations = "
#define S_SECTIONHEADERPOINTERTOLINENUMBERS				"PointerToLinenumbers = "
#define S_SECTIONHEADERNUMBEROFRELOCATIONS				"NumberOfRelocations = "
#define S_SECTIONHEADERNUMBEROFLINENUMBERS				"NumberOfLinenumbers = "
#define S_SECTIONHEADERCHARACTERISTICS					"Characteristics = "

#define S_DOSSTUBCODETITLE								"DOS stub code"

#define S_FORWARD										"Forward"
#define S_BACK											"Back"
#define S_SWITCH										"Switch"