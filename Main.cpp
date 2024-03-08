# include "Recursion.h"
#include <cassert>

void Recursion(TiXmlElement* Element, CStdioFile* CSVfile, bool descent, const CString Source_type, CString& Nature_Name, int& Nature_count, CString& m_Lw)//L'idée de la descente= 3h de cogitation :-(
{
	CString Csv_Source_Info = "";
	CString Val_Elem = "";

	while (Element)
	{
		Val_Elem = Element->Value();
		//I---------------------Gestion des spécificités des balises--------------------------------------
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
				(*CSVfile).WriteString(Csv_Source_Info);
			}
			else
			{
				Csv_Source_Info = "\n" + Nature_Name + ";";
				(*CSVfile).WriteString(Csv_Source_Info);
				Csv_Source_Info = "";
			}
			Nature_count++;
		}
		// Regroupement des infos géométriques
		if (Val_Elem == "Points" || Val_Elem == "Point" && Source_type == XML_ECHANG_VALEUR_SP)
		{
			TiXmlPrinter printer;
			Element->Accept(&printer);
			CString geom = printer.CStr();
			geom.Remove('\n');
			(*CSVfile).WriteString(geom + ";");
			return;
		}

		if (Val_Elem == "Ref_Model")
			m_Lw = Find_Lw(Element->GetText()) + ";";

		// Si Lw existe on le veut en bloc dans une colonne du CSV
		if (Val_Elem == "Lw")
		{
			TiXmlPrinter printer;
			Element->Accept(&printer);
			CString Lw = printer.CStr();
			Lw.Remove('\n');
			(*CSVfile).WriteString(Lw + ";");
		}

		TiXmlAttribute* Attribute_finder = Element->FirstAttribute();
		while (Attribute_finder && !(Val_Elem == Nature[Source_type]) && !(Val_Elem == "Lw") && !(std::strcmp(Attribute_finder->Name(), "Percent") == 0))//Tant qu'il y a des attributs on les parse
		{
			Csv_Source_Info = Attribute_finder->Value() + CString(";");
			(*CSVfile).WriteString(Csv_Source_Info);
			if (Val_Elem == "Period" && (Element->GetText() == NULL))
				(*CSVfile).WriteString(m_Lw);
			Attribute_finder = Attribute_finder->Next();
			Csv_Source_Info = "";
		}

		if (!(Val_Elem == "Measure") && !(Element->GetText() == NULL) && !(Val_Elem == "Lw") && !(Val_Elem == "Ref_Model") && !(Val_Elem == "Directivity"))//On regarde s''il y a du texte dans l'élément
		{
			Csv_Source_Info = Csv_Source_Info + (CString)(Element->GetText()) + ";";
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
			// II----------------Promenade récursive-----------------------------
			// 
			//Si texte: on est sur une feuille, donc on regarde l'élément suivant ou on remonte dans l'arborescence
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
		}
		else//L'élément n'a pas de texte 
			//On essaie de descendre 
			// ou on va sur l'élément suivant ou on remonte et on va voir le suivant si cela n'a pas déja été fait (!descent). 
		{
			Recursion(Element->FirstChildElement(), CSVfile, true, Source_type, Nature_Name, Nature_count, m_Lw);
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count, m_Lw);
		}
		return;
	}
}

void Find_headers(TiXmlElement* Element, CStdioFile* CSVfile, bool descent, const CString Source_type, int& Source_Count)//L'idée de la descente= 3h de cogitation :-(
{
	CString Csv_Source_Info = "";
	CString Val_Elem = "";

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
			(*CSVfile).WriteString(Val_Elem);
			(*CSVfile).WriteString((LPCTSTR)";");
		}

		Csv_Source_Info = "";
		if (Val_Elem == "Points" || Val_Elem == "Point" && Source_type == XML_ECHANG_VALEUR_SP)
		{
			CString rvalueCompensator = "Geometry;";
			(*CSVfile).WriteString(rvalueCompensator);
			return;
		}

		if (Val_Elem == "Lw")
		{
			(*CSVfile).WriteString((LPCTSTR)"Lw;");
			Find_headers(Element->NextSiblingElement(), CSVfile, descent, Source_type, Source_Count);//On peut lancer un next, la recursion s'arrete, il n'y aura pas de second parcours
		}
		TiXmlAttribute* Attribute_finder = Element->FirstAttribute();
		while (Attribute_finder && !(Val_Elem == Nature[Source_type]) && !(Val_Elem == "Lw"))//Tant qu'il y a des attributs on les parse
		{
			Csv_Source_Info = Csv_Source_Info + Attribute_finder->Name() + ";";
			Attribute_finder = Attribute_finder->Next();
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
		}
		if (!(Val_Elem == "Measure") && !(Element->GetText() == NULL) && !(Val_Elem == "Directivity"))//On regarde s''il y a du texte dans l'élément
		{
			Csv_Source_Info = Csv_Source_Info + Val_Elem + ";";
			(*CSVfile).WriteString(Csv_Source_Info);
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

CString Find_Lw(std::string id)
{
	const std::string REF_FILE = "C:\\Users\\dcollado\\Desktop\\points_RefModel.xml";
	const std::string ALT_REF_FILE = "C:\\Users\\dcollado\\AppData\\Local\\MITHRA\\MITHRA\-SIG\\Params\\MITHRA\-SIG_RefModel.xml";

	TiXmlDocument doc(REF_FILE.c_str());
	TiXmlDocument Alt_doc(ALT_REF_FILE.c_str());
	if (!doc.LoadFile()) { return "Echec"; };
	TiXmlHandle MyHandle(&doc);
	TiXmlElement* source_element = MyHandle.FirstChildElement("Sources_Industry").FirstChildElement("sources").FirstChildElement("source").Element();//Première Nature
	while (source_element)
	{
		if (strcmp(source_element->Attribute("id"), id.c_str()) == 0)
		{
			TiXmlPrinter printer;
			source_element->FirstChildElement("Lw")->Accept(&printer);
			CString geom = printer.CStr();
			geom.Remove('\n');
			return geom;
		}
		else
			source_element = source_element->NextSiblingElement();
	}
	if (!Alt_doc.LoadFile()) { return "Echec"; };
	TiXmlHandle Alt_MyHandle(&Alt_doc);
	source_element = Alt_MyHandle.FirstChildElement("Sources_Industry").FirstChildElement("sources").FirstChildElement("source").Element();//Première Nature
	while (source_element)
	{
		if (strcmp(source_element->Attribute("id"), id.c_str()) == 0)
		{
			TiXmlPrinter printer;
			source_element->FirstChildElement("Lw")->Accept(&printer);
			CString geom = printer.CStr();
			geom.Remove('\n');
			return geom;
		}
		else
			source_element = source_element->NextSiblingElement();
	}
	return "Spectrum Not Found";
}

int main()
{
	CStdioFile fileResCSV;
	const std::string TEST_FILE = "C:\\Users\\dcollado\\Desktop\\points.xml";
	const CString FLAT_FILE = "C:\\Users\\dcollado\\Desktop\\points.csv";
	CString m_Lw = "";
	TiXmlDocument doc = TiXmlDocument(TEST_FILE.c_str());
	if (doc.LoadFile())
	{
		if (!fileResCSV.Open(FLAT_FILE, CFile::modeCreate | CFile::modeWrite))
		{
			return 0;
		}

		TiXmlElement* root = doc.FirstChildElement();//XML_Sources
		TiXmlElement* Nature_root = root->FirstChildElement()->NextSiblingElement()->FirstChildElement();//Première Nature
		CString Nature_Name = "";
		int Nature_count = 0;
		int Source_Count = 0;
		//Une ligne CSV par Child de l'élément envoyé à Recursion et de ceux de ses Siblings
		Find_headers(Nature_root, &fileResCSV, false, XML_ECHANG_VALEUR_SP, Source_Count);
		CString newline = "\n";
		(fileResCSV).WriteString(newline);
		Recursion(Nature_root, &fileResCSV, false, XML_ECHANG_VALEUR_SP, Nature_Name, Nature_count, m_Lw);

	}

	fileResCSV.Close();
	return 0;
}