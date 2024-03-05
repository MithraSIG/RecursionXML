# include "Recursion.h"

void Recursion(TiXmlElement* Element, CStdioFile* CSVfile, bool descent, const CString Source_type)//L'idée de la descente= 3h de cogitation :-(
{
	CString Csv_Source_Info = "";
	CString Val_Elem = "";
	static CString Nature_Name = "";
	static int Nature_count = 0;
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

		TiXmlAttribute* Attribute_finder = Element->FirstAttribute();
		while (Attribute_finder && !(Val_Elem == Nature[Source_type]))//Tant qu'il y a des attributs on les parse
		{
			Csv_Source_Info = Csv_Source_Info + Attribute_finder->Value() + ";";
			Attribute_finder = Attribute_finder->Next();
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
		}
		if (!(Element->GetText() == NULL))//On regarde s''il y a du texte dans l'élément
		{
			Csv_Source_Info = Csv_Source_Info + (CString)(Element->GetText()) + ";";
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type);
		}
		else//L'élément n'a pas de texte 
		{
			Recursion(Element->FirstChildElement(), CSVfile, true, Source_type);
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type);
		}
		return;
	}
}


void Find_headers(TiXmlElement* Element, CStdioFile* CSVfile, bool descent, const CString Source_type)
{
	CString Csv_Source_Info = "", Val_Elem;
	while (Element)
	{
		Val_Elem = Element->Value();
		if (Val_Elem == Source_type)
			Csv_Source_Info = "\n ";
		if (Val_Elem == "Points" || Val_Elem == "Point" && Source_type == XML_ECHANG_VALEUR_SP)
		{
			TiXmlPrinter printer;
			Element->Accept(&printer);
			CString geom = printer.CStr();
			geom.Remove('\n');
			(*CSVfile).WriteString(geom + ";");
			return;
		}

		TiXmlAttribute* Attribute_finder = Element->FirstAttribute();
		while (Attribute_finder)//Tant qu'il y a des attributs on les parse
		{
			Csv_Source_Info = Csv_Source_Info + Attribute_finder->Value() + ";";
			Attribute_finder = Attribute_finder->Next();
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
		}
		if (!(Element->GetText() == NULL))//On regarde s''il y a du texte dans l'élément
		{
			Csv_Source_Info = Csv_Source_Info + (CString)(Element->GetText()) + ";";
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type);
		}
		else//L'élément n'a pas de texte 
		{
			Recursion(Element->FirstChildElement(), CSVfile, true, Source_type);
			Recursion(Element->NextSiblingElement(), CSVfile, descent, Source_type);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent, Source_type);
		}
		return;
	}
}
int main()
{
	CStdioFile fileResCSV;
	const char* TEST_FILE = "C:\\Users\\dcollado\\Desktop\\volum.xml";
	const CString FLAT_FILE = "C:\\Users\\dcollado\\Desktop\\volum.csv";

	TiXmlDocument doc = TiXmlDocument(TEST_FILE);
	if (doc.LoadFile())
	{
		if (!fileResCSV.Open(FLAT_FILE, CFile::modeCreate | CFile::modeWrite))
		{
			return 0;
		}

		TiXmlElement* root = doc.FirstChildElement();//XML_Sources
		static  TiXmlElement* Nature_root = root->FirstChildElement()->NextSiblingElement()->FirstChildElement();//Première Nature
		//Une ligne CSV par Child de l'élément envoyé à Recursion et de ceux de ses Siblings
		//Find_headers(Nature_root, &fileResCSV, false, XML_ECHANG_VALEUR_SVol);
		Recursion(Nature_root, &fileResCSV, false, XML_ECHANG_VALEUR_SVol);
		
	}

	fileResCSV.Close();
	return 0;
}