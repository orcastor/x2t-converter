import sys
sys.path.append("../../../../../build_tools/scripts")
sys.path.append("../../../../Common/js")
import base
import common

base.configure_common_apps()

if not base.is_dir("xml"):
  base.copy_dir("../../../xml", "./xml")
  base.replaceInFile("./xml/libxml2/libxml.h", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
  base.replaceInFile("./xml/libxml2/xmlIO.c", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
  base.replaceInFile("./xml/src/xmllight_private.h", "#include \"../../common/", "#include \"../../../../../common/")
  base.replaceInFile("./xml/src/xmllight_private.h", "#include \"../../../UnicodeConverter/", "#include \"../../../../../../UnicodeConverter/")
  base.replaceInFile("./xml/src/xmllight_private.h", "#include <boost/lexical_cast.hpp>", "")
  base.replaceInFile("./xml/src/xmllight_private.h", "boost::lexical_cast<int>", "std::stoi")
  base.replaceInFile("./xml/include/xmlutils.h", "#include \"../../common/", "#include \"../../../../../common/")

if not base.is_dir("freetype-2.10.4"):
  base.copy_dir("../../../freetype-2.10.4", "./freetype-2.10.4")
  # smooth
  base.copy_file("./freetype-2.10.4/src/smooth/ftgrays.c", "./freetype-2.10.4/src/smooth/ftgrays.cpp")
  common.apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays1.patch")
  common.apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays2.patch")
  common.apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays3.patch")
  base.copy_file("./freetype-2.10.4/src/smooth/smooth.c", "./freetype-2.10.4/src/smooth/smooth.cpp")
  common.apply_patch("./freetype-2.10.4/src/smooth/smooth.cpp", "./wasm/patches/smooth.patch")
  # ftobjs
  common.apply_patch("./freetype-2.10.4/src/base/ftobjs.c", "./wasm/patches/ftobjs1.patch")
  common.apply_patch("./freetype-2.10.4/src/base/ftobjs.c", "./wasm/patches/ftobjs2.patch")
  # ttcmap
  base.copy_file("./freetype-2.10.4/src/sfnt/ttcmap.c", "./freetype-2.10.4/src/sfnt/ttcmap.cpp")
  common.apply_patch("./freetype-2.10.4/src/sfnt/ttcmap.cpp", "./wasm/patches/ttcmap.patch")
  base.copy_file("./freetype-2.10.4/src/sfnt/sfnt.c", "./freetype-2.10.4/src/sfnt/sfnt.cpp")
  common.apply_patch("./freetype-2.10.4/src/sfnt/sfnt.cpp", "./wasm/patches/sfnt.patch")
  common.apply_patch("./freetype-2.10.4/builds/unix/ftsystem.c", "./wasm/patches/ftsystem.patch")

def apply_font_patch(file_name, arr_name):
  file_content = base.readFile("../../../../PdfReader/Resources/CMap/" + file_name)
  arr_str_cidToUnicode = file_content.splitlines()
  arr_int_cidToUnicode = [int(i, 16) for i in arr_str_cidToUnicode]
  res_content = ("#ifndef _PDF_READER_RESOURCE_FONT_" + arr_name + "\n#define _PDF_READER_RESOURCE_FONT_" + arr_name + "\nnamespace PdfReader\n{\nstatic const unsigned int c_arr" + arr_name + "[]={")
  res_content += ','.join(map(str, arr_int_cidToUnicode))
  res_content += ("};\nstatic const unsigned int c_n" + arr_name + "=" + str(len(arr_int_cidToUnicode)) + ";\n}\n#endif //_PDF_READER_RESOURCE_FONT_" + arr_name + "\n")
  base.writeFile("./fonts/" + file_name + ".h", res_content)

if not base.is_dir("fonts"):
  base.create_dir("./fonts")
  apply_font_patch("Adobe-GB1.cidToUnicode", "Adobe_GB1")
  apply_font_patch("Adobe-Korea1.cidToUnicode", "Adobe_Korea1")
  apply_font_patch("Adobe-KR.cidToUnicode", "Adobe_KR")
  apply_font_patch("Adobe-Japan1.cidToUnicode", "Adobe_Japan1")

base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n    UDataMemory tData;", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;")
