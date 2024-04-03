#pragma once
#include "Recursion.h"
using namespace tinyxml2;
using namespace std;
using namespace gmd;

std::string Find_Lw(std::string id)
{
	const std::string REF_FILE = "C:\\Users\\dcollado\\Desktop\\test_RefModel.xml";
	//const std::string REF_FILE = "C:\\Users\\dcollado\\Desktop\\point_RefModel.xml";
	const std::string ALT_REF_FILE = "C:\\Users\\dcollado\\AppData\\Local\\MITHRA\\MITHRA\-SIG\\Params\\MITHRA\-SIG_RefModel.xml";

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(REF_FILE.c_str()) != tinyxml2::XML_SUCCESS)
		return "Echec";

	XMLHandle MyHandle(&doc);
	XMLElement* source_element = MyHandle.FirstChildElement("Sources_Industry").FirstChildElement("sources").FirstChildElement("source").ToElement();//Première Nature
	while (source_element)
	{
		if (strcmp(source_element->Attribute("id"), id.c_str()) == 0)
		{
			XMLPrinter printer;
			source_element->FirstChildElement("Lw")->Accept(&printer);
			std::string geom = printer.CStr();
			String::replace(geom, "\n", "");
			return geom;
		}
		else
			source_element = source_element->NextSiblingElement();
	}
	tinyxml2::XMLDocument Alt_doc(ALT_REF_FILE.c_str());
	if (Alt_doc.LoadFile(REF_FILE.c_str()) != tinyxml2::XML_SUCCESS)
		return "Echec";

	XMLHandle Alt_MyHandle(&Alt_doc);
	source_element = Alt_MyHandle.FirstChildElement("Sources_Industry").FirstChildElement("sources").FirstChildElement("source").ToElement();//Première Nature
	while (source_element)
	{
		if (strcmp(source_element->Attribute("id"), id.c_str()) == 0)
		{
			XMLPrinter printer;
			source_element->FirstChildElement("Lw")->Accept(&printer);
			std::string geom = printer.CStr();
			String::replace(geom, "\n", "");
			return geom;
		}
		else
			source_element = source_element->NextSiblingElement();
	}
	return "Spectrum Not Found";
}

void Recursion(XMLElement* Element, ofstream& CSVfile, bool descent, const std::string Source_type, std::string& Nature_Name, int& Nature_count, std::string& m_Lw)
{
	std::string Csv_Source_Info = "";
	std::string Val_Elem = "";

	while (Element)
	{
		Val_Elem = Element->Value();
		//-----------------------------------------------------------------------------------------------
	//   I---------------------Gestion des spécificités des balises--------------------------------------
		//Nom de la nature
		if (Val_Elem == Nature[Source_type])
		{
			Nature_Name = Element->FirstAttribute()->Value();
		}
		if (Val_Elem == Source_type)
		{
			if (Nature_count == 0)//A la premiere source, on ne va pas à la ligne
			{
				Csv_Source_Info = Nature_Name + ";";
				CSVfile << Csv_Source_Info;
			}
			else
			{
				Csv_Source_Info = "\n" + Nature_Name + ";";
				CSVfile << Csv_Source_Info;
				Csv_Source_Info = "";
			}
			Nature_count++;
		}
		// Regroupement des infos géométriques
		if (Val_Elem == "Points" || Val_Elem == "Point" && Source_type == XML_ECHANG_VALEUR_SP)
		{
			XMLPrinter printer;
			Element->Accept(&printer);
			std::string geom = printer.CStr();
			String::replace(geom, "\n", "");
			CSVfile << geom + ";";
			return;
		}
		//Si on a une ref. on va la chercher dans le fichier correspondant
		if (Val_Elem == "Ref_Model")
			m_Lw = Find_Lw(Element->GetText()) + ";";

		// Si Lw existe on le veut en bloc dans une colonne du CSV
		if (Val_Elem == "Lw")
		{
			XMLPrinter printer;
			Element->Accept(&printer);
			std::string Lw = printer.CStr();
			String::replace(Lw, "\n", "");
			CSVfile << Lw + ";";
		}

		const XMLAttribute* Attribute_finder = Element->FirstAttribute();
		//Tant qu'il y a des attributs non Lw, non nature, et qui ne soit pas  "Percent",  on les parse
		while (Attribute_finder && !(Val_Elem == Nature[Source_type]) && !(Val_Elem == "Lw") && !(std::strcmp(Attribute_finder->Name(), "Percent") == 0))
		{
			Csv_Source_Info = Attribute_finder->Value() + std::string(";");
			CSVfile << Csv_Source_Info;
			//Period a deux cas différents: avec ou sans refernec au modele, ici on caracterise  le Period de la reference
			if (Val_Elem == "Period" && (Element->GetText() == NULL) && !(Element->FirstChildElement()))
				CSVfile << m_Lw;
			Attribute_finder = Attribute_finder->Next();
			Csv_Source_Info = "";
		}
		//Quand il a du texte
		if (!(Val_Elem == "Measure") && !(Element->GetText() == NULL) && !(Val_Elem == "Lw") && !(Val_Elem == "Ref_Model") && !(Val_Elem == "Directivity"))//On regarde s''il y a du texte dans l'élément
		{
			Csv_Source_Info = Csv_Source_Info + (std::string)(Element->GetText()) + ";";
			CSVfile << Csv_Source_Info;
			Csv_Source_Info = "";
		}

		//--------------------------------------------------------------------
		//  II----------------Promenade récursive-----------------------------
		if (!(Element->GetText() == NULL))
		{
			//Si texte: on est sur une feuille, donc on regarde l'élément suivant ou on remonte dans l'arborescence
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
			if (!descent)
				Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
		}
		else//L'élément n'a pas de texte 
			//On essaie de descendre 
			// ou on va sur l'élément suivant ou on remonte et on va voir le suivant si cela n'a pas déja été fait (!descent). 
		{
			Recursion(Element->FirstChildElement(), CSVfile, true, Source_type, Nature_Name, Nature_count, m_Lw);
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
			if (!descent)
				Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
		}
		return;
	}
}

void Find_headers(XMLElement* Element, ofstream& CSVfile, bool descent, const std::string Source_type, int& Source_Count)
{
	std::string Csv_Source_Info = "";
	std::string Val_Elem = "";

	while (Element)
	{
		Val_Elem = Element->Value();
		if (Val_Elem == Source_type)
		{
			Source_Count++;
		}
		if (Source_Count > 1) return;
		if (Val_Elem == Nature[Source_type])
		{
			if (Source_Count > 0) return;
			CSVfile << Val_Elem;
			CSVfile << ";";
		}

		Csv_Source_Info = "";
		if (Val_Elem == "Points" || Val_Elem == "Point" && Source_type == XML_ECHANG_VALEUR_SP)
		{
			CSVfile << "Geometry;";
			return;
		}

		if (Val_Elem == "Lw")
		{
			CSVfile << "Lw;";
		}
		const XMLAttribute* Attribute_finder = Element->FirstAttribute();
		while (Attribute_finder && !(Val_Elem == Nature[Source_type]) && !(Val_Elem == "Lw"))//Tant qu'il y a des attributs on les parse
		{
			if (std::strcmp(Attribute_finder->Name(), "Percent") == 0)
				Csv_Source_Info = Csv_Source_Info + "Lw" + ";";
			else
			{
				Csv_Source_Info = Csv_Source_Info + Attribute_finder->Name() + ";";
			}
			Attribute_finder = Attribute_finder->Next();
			CSVfile << Csv_Source_Info;
			Csv_Source_Info = "";
		}
		if (!(Val_Elem == "Ref_Model") && !(Val_Elem == "Measure") && !(Element->GetText() == NULL) && !(Val_Elem == "Directivity"))//On regarde s''il y a du texte dans l'élément
		{
			Csv_Source_Info = Csv_Source_Info + Val_Elem + ";";
			CSVfile << Csv_Source_Info;
			Csv_Source_Info = "";
			Find_headers(Element->NextSiblingElement(), CSVfile, descent, Source_type, Source_Count);
			if (!descent) Find_headers(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Source_Count);
		}
		else//L'élément n'a pas de texte 
		{
			Find_headers(Element->FirstChildElement(), CSVfile, true, Source_type, Source_Count);
			Find_headers(Element->NextSiblingElement(), CSVfile, descent, Source_type, Source_Count);
			if (!descent) Find_headers(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Source_Count);
		}
		return;
	}
}



int main()
{
	ofstream fileResCSV;
	const std::string TEST_FILE = "C:\\Users\\dcollado\\Desktop\\test.xml";
	const std::string FLAT_FILE = "C:\\Users\\dcollado\\Desktop\\test_mano.csv";
	std::string m_Lw = "";
	tinyxml2::XMLDocument doc ;
	if (doc.LoadFile(TEST_FILE.c_str()) == tinyxml2::XML_SUCCESS)
	{
		fileResCSV.open(FLAT_FILE);
		if (!fileResCSV.is_open())
		{
			return 0;
		}

		XMLElement* root = doc.FirstChildElement();//XML_Sources
		XMLElement* Nature_root = root->FirstChildElement()->NextSiblingElement()->FirstChildElement();//Première Nature
		std::string Nature_Name = "";
		int Nature_count = 0;
		int Source_Count = 0;
		std::string m_Lw = "";
		//Une ligne CSV par Child de l'élément envoyé à Recursion et de ceux de ses Siblings
		Find_headers(Nature_root, fileResCSV, false, XML_ECHANG_VALEUR_SVol, Source_Count);
		std::string newline = "\n";
		fileResCSV << newline;
		Recursion(Nature_root, fileResCSV, false, XML_ECHANG_VALEUR_SVol, Nature_Name, Nature_count, m_Lw);

	}

	fileResCSV.close();
	return 0;
}