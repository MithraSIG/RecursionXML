#pragma once
#include "tinyxml.h"
#include<afx.h>
#include <stdio.h>
#include <iostream>
#include<string>


#include <map>

CString XML_ECHANG_BALISE_Natures_SL = "SL_Natures";
CString XML_ECHANG_BALISE_Nature_SL = "SL_Nature";
CString XML_ECHANG_VALEUR_SL = "SL";
CString XML_ECHANG_BALISE_Natures_SP = "SP_Natures";
CString XML_ECHANG_BALISE_Nature_SP = "SP_Nature";
CString XML_ECHANG_VALEUR_SP = "SP";
CString XML_ECHANG_BALISE_Natures_SFac = "SFac_Natures";
CString XML_ECHANG_BALISE_Nature_SFac = "SFac_Nature";
CString XML_ECHANG_VALEUR_SFac = "SFac";
CString XML_ECHANG_BALISE_Natures_SVol = "SVol_Natures";
CString XML_ECHANG_BALISE_Nature_SVol = "SVol_Nature";
CString XML_ECHANG_VALEUR_SVol = "SVol";
CString XML_ECHANG_BALISE_Natures_SSurf = "SSurf_Natures";
CString XML_ECHANG_BALISE_Nature_SSurf = "SSurf_Nature";
CString XML_ECHANG_VALEUR_SSurf = "SSurf";

std::map<  CString, CString> Natures{
   {XML_ECHANG_BALISE_Nature_SL,XML_ECHANG_BALISE_Natures_SL},
   {XML_ECHANG_BALISE_Nature_SP, XML_ECHANG_BALISE_Natures_SP},
   {XML_ECHANG_BALISE_Nature_SFac,XML_ECHANG_BALISE_Natures_SFac},
   {XML_ECHANG_BALISE_Nature_SVol,XML_ECHANG_BALISE_Natures_SVol},
   {XML_ECHANG_BALISE_Nature_SSurf,XML_ECHANG_BALISE_Natures_SSurf}
};
std::map< CString, CString> Nature{
   {XML_ECHANG_VALEUR_SL,XML_ECHANG_BALISE_Nature_SL} ,
   {XML_ECHANG_VALEUR_SP,XML_ECHANG_BALISE_Nature_SP},
   {XML_ECHANG_VALEUR_SFac,XML_ECHANG_BALISE_Nature_SFac},
   {XML_ECHANG_VALEUR_SVol,XML_ECHANG_BALISE_Nature_SVol},
   {XML_ECHANG_VALEUR_SSurf,XML_ECHANG_BALISE_Nature_SSurf}
};

static int compteur = 0;
void Find_headers(TiXmlElement* node, CStdioFile*, bool,  CString,int&);
void Recursion(TiXmlElement* node, CStdioFile*, bool,  CString ,CString&, int&);
CString Find_Lw(const char*);

