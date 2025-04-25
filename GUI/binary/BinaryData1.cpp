/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace frontend
{

//================== index.html ==================
static const unsigned char temp_binary_data_0[] =
"<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"/><link rel=\"icon\" href=\"/favicon.ico\"/><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"/><meta name=\"theme-color\" content=\"#000000\"/><meta name=\"description"
"\" content=\"Web site created using create-react-app\"/><link rel=\"apple-touch-icon\" href=\"/logo192.png\"/><link rel=\"manifest\" href=\"/manifest.json\"/><title>MidiRouter</title><script defer=\"defer\" src=\"/static/js/main.10d3e220.js\"></scrip"
"t><link href=\"/static/css/main.2c4a4183.css\" rel=\"stylesheet\"></head><body><noscript>You need to enable JavaScript to run this app.</noscript><div id=\"root\"></div></body></html>";

const char* index_html = (const char*) temp_binary_data_0;

}

#include "MyBinaryData.h"

namespace frontend
{

const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x2c834af8:  numBytes = 645; return index_html;
        case 0x1a905808:  numBytes = 1047; return assetmanifest_json;
        case 0x3391daa9:  numBytes = 205429; return main_10d3e220_js;
        case 0x9f388cbc:  numBytes = 1210; return main_10d3e220_js_LICENSE_txt;
        case 0x0c8f32c6:  numBytes = 944437; return main_10d3e220_js_map;
        case 0x8b3cd2ff:  numBytes = 0; return main_29959fd7_js;
        case 0xd633b412:  numBytes = 0; return main_29959fd7_js_LICENSE_txt;
        case 0xb9d8e720:  numBytes = 7167; return main_2c4a4183_css;
        case 0xc6fd2bbd:  numBytes = 18383; return main_2c4a4183_css_map;
        case 0x2af5ce26:  numBytes = 1684624; return RobotoFlexVariableFont_f700cdc3d6dd6bdaebd3_ttf;
        case 0x6961eb90:  numBytes = 438; return add_68e829718e2d9bcb7b750e80251ec33f_svg;
        case 0x369df5b5:  numBytes = 781; return load_9cde5005b4baee7dadea5456adaf49ce_svg;
        case 0x0c80fd73:  numBytes = 66864; return overpassextrabold_3b0b89804112a9ae91c9_otf;
        case 0xf62828fe:  numBytes = 70300; return overpassheavy_de2a6b0b6ade2c8cbe17_otf;
        case 0xebf73d07:  numBytes = 69728; return overpassregular_3f851cebdc18b56d2f14_otf;
        case 0xdd399959:  numBytes = 674; return save2_f6c7671a3716f53f38411508c67d3a4b_svg;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "index_html",
    "assetmanifest_json",
    "main_10d3e220_js",
    "main_10d3e220_js_LICENSE_txt",
    "main_10d3e220_js_map",
    "main_29959fd7_js",
    "main_29959fd7_js_LICENSE_txt",
    "main_2c4a4183_css",
    "main_2c4a4183_css_map",
    "RobotoFlexVariableFont_f700cdc3d6dd6bdaebd3_ttf",
    "add_68e829718e2d9bcb7b750e80251ec33f_svg",
    "load_9cde5005b4baee7dadea5456adaf49ce_svg",
    "overpassextrabold_3b0b89804112a9ae91c9_otf",
    "overpassheavy_de2a6b0b6ade2c8cbe17_otf",
    "overpassregular_3f851cebdc18b56d2f14_otf",
    "save2_f6c7671a3716f53f38411508c67d3a4b_svg"
};

const char* originalFilenames[] =
{
    "index.html",
    "asset-manifest.json",
    "main.10d3e220.js",
    "main.10d3e220.js.LICENSE.txt",
    "main.10d3e220.js.map",
    "main.29959fd7.js",
    "main.29959fd7.js.LICENSE.txt",
    "main.2c4a4183.css",
    "main.2c4a4183.css.map",
    "RobotoFlex-VariableFont.f700cdc3d6dd6bdaebd3.ttf",
    "add.68e829718e2d9bcb7b750e80251ec33f.svg",
    "load.9cde5005b4baee7dadea5456adaf49ce.svg",
    "overpass-extrabold.3b0b89804112a9ae91c9.otf",
    "overpass-heavy.de2a6b0b6ade2c8cbe17.otf",
    "overpass-regular.3f851cebdc18b56d2f14.otf",
    "save2.f6c7671a3716f53f38411508c67d3a4b.svg"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
