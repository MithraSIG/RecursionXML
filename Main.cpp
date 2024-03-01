# include "Recursion.h"

const char* Source_type = "SVol";
void Recursion(TiXmlElement* Element, CStdioFile* CSVfile, bool descent)
{
	CString Csv_Source_Info ="",test;
	if (Element)
		 test = Element->Value();
		if (test == Source_type)
			Csv_Source_Info = "\n ";
		

	while (Element)
	{
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
			Recursion(Element->NextSiblingElement(), CSVfile, descent);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent);
		}
		else//L'élément n'a pas de texte 
		{
			Recursion(Element->FirstChildElement(), CSVfile, true);
			Recursion(Element->NextSiblingElement(), CSVfile, descent);
			if (!descent) Recursion(Element->Parent()->NextSiblingElement(), CSVfile, descent);
		}
		return;
	}
}


void Recursion_(TiXmlElement* Element, CStdioFile CSVfile)
{
	printf("%s", Element->FirstChildElement()->Value());
	return;
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
		Recursion(Nature_root, &fileResCSV, false);
	}

	fileResCSV.Close();
	return 0;
}