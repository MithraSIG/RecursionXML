# include "Recursion.h"
#include <cassert>

void Recursion(TiXmlElement* Element, CStdioFile* CSVfile, bool descent, const CString Source_type, CString& Nature_Name, int& Nature_count)//L'idée de la descente= 3h de cogitation :-(
{
	CString Csv_Source_Info = "";
	CString Val_Elem = "";

	while (Element)
	{
		Val_Elem = Element->Value();

		if (Val_Elem == Nature[Source_type])
		{
			Nature_Name = Element->FirstAttribute()->Value();

		}
		if (Val_Elem == Source_type)
		{
			if (Nature_count == 0)//A la premiere source, on ne va pas à la ligne
			{
				Csv_Source_Info = Nature_Name + ";";
			}
			else
			{
				Csv_Source_Info = "\n" + Nature_Name + ";";
				(*CSVfile).WriteString(Csv_Source_Info);
				Csv_Source_Info = "";
			}
			Nature_count++;
		}

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
			(*CSVfile).WriteString(Find_Lw(Element->GetText()) + ";");

		if (Val_Elem == "Lw")
		{
			TiXmlPrinter printer;
			Element->Accept(&printer);
			CString Lw = printer.CStr();
			Lw.Remove('\n');
			(*CSVfile).WriteString(Lw + ";");
		}

		TiXmlAttribute* Attribute_finder = Element->FirstAttribute();
		while (Attribute_finder && !(Val_Elem == Nature[Source_type])  && !(Val_Elem== "Lw"))//Tant qu'il y a des attributs on les parse
		{
			Csv_Source_Info = Csv_Source_Info + Attribute_finder->Value() + ";";
			Attribute_finder = Attribute_finder->Next();
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
		}
		if (!(Val_Elem == "Measure") && !(Element->GetText() == NULL) && !(Val_Elem == "Lw"))//On regarde s''il y a du texte dans l'élément
		{
			Csv_Source_Info = Csv_Source_Info + (CString)(Element->GetText()) + ";";
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count);
		}
		else//L'élément n'a pas de texte 
		{
			Recursion(Element->FirstChildElement(), CSVfile, true, Source_type, Nature_Name, Nature_count);
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type, Nature_Name, Nature_count);
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
		if (!(Val_Elem == "Measure") && !(Element->GetText() == NULL))//On regarde s''il y a du texte dans l'élément
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
	const std::string REF_FILE = "C:\\Users\\dcollado\\Desktop\\volum_RefModel.xml";
	TiXmlDocument doc(REF_FILE.c_str());
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
}

int main()
{
	CStdioFile fileResCSV;
	const std::string TEST_FILE = "C:\\Users\\dcollado\\Desktop\\volum.xml";
	const CString FLAT_FILE = "C:\\Users\\dcollado\\Desktop\\volum.csv";

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
		Find_headers(Nature_root, &fileResCSV, false, XML_ECHANG_VALEUR_SVol, Source_Count);
		CString newline = "\n";
		(fileResCSV).WriteString(newline);
		Recursion(Nature_root, &fileResCSV, false, XML_ECHANG_VALEUR_SVol, Nature_Name, Nature_count);

	}

	fileResCSV.Close();
	return 0;
}