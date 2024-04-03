#pragma once
#include "tinyxml2.h"
#include"GmdCast.h"
#include<afx.h>
#include <stdio.h>
#include<string>
#include <fstream>
#include<iostream>
#include <cassert>

#include <map>

const std::string XML_ECHANG_BALISE_Natures_SL = "SL_Natures";
const std::string XML_ECHANG_BALISE_Nature_SL = "SL_Nature";
const std::string XML_ECHANG_VALEUR_SL = "SL";
const std::string XML_ECHANG_BALISE_Natures_SP = "SP_Natures";
const std::string XML_ECHANG_BALISE_Nature_SP = "SP_Nature";
const std::string XML_ECHANG_VALEUR_SP = "SP";
const std::string XML_ECHANG_BALISE_Natures_SFac = "SFac_Natures";
const std::string XML_ECHANG_BALISE_Nature_SFac = "SFac_Nature";
const std::string XML_ECHANG_VALEUR_SFac = "SFac";
const std::string XML_ECHANG_BALISE_Natures_SVol = "SVol_Natures";
const std::string XML_ECHANG_BALISE_Nature_SVol = "SVol_Nature";
const std::string XML_ECHANG_VALEUR_SVol = "SVol";
const std::string XML_ECHANG_VALEUR_SSurf = "SSurf";
const std::string XML_ECHANG_BALISE_Natures_SSurf = "SSurf_Natures";
const std::string XML_ECHANG_BALISE_Nature_SSurf = "SSurf_Nature";



std::map< std::string, std::string> Nature{
   {XML_ECHANG_VALEUR_SL,XML_ECHANG_BALISE_Nature_SL} ,
   {XML_ECHANG_VALEUR_SP,XML_ECHANG_BALISE_Nature_SP},
   {XML_ECHANG_VALEUR_SFac,XML_ECHANG_BALISE_Nature_SFac},
   {XML_ECHANG_VALEUR_SVol,XML_ECHANG_BALISE_Nature_SVol},
   {XML_ECHANG_VALEUR_SSurf,XML_ECHANG_BALISE_Nature_SSurf}
};


