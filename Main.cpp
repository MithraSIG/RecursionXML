# include "Recursion.h"

void Recursion(TiXmlElement* Element, CStdioFile* CSVfile)
{
	CString Csv_Source_Info = "";
	while (Element)
	{
		TiXmlElement* Source_root = Element->FirstChildElement();//1ere Source
		while (Source_root)
		{
			Csv_Source_Info = Csv_Source_Info + Source_root->Value() + "\n";
			(*CSVfile).WriteString(Csv_Source_Info);
			Csv_Source_Info = "";
			Source_root = Source_root->NextSiblingElement();
		}

		//On passe à la nature suivante
		Element = Element->NextSiblingElement();

	}
}

void main() {
	CStdioFile fileResCSV;
	const char* TEST_FILE = "C:\\Users\\dcollado\\Desktop\\point.xml";
	const CString FLAT_FILE = "C:\\Users\\dcollado\\Desktop\\point.csv";

	TiXmlDocument doc = TiXmlDocument(TEST_FILE);
	if (doc.LoadFile())
	{
		if (!fileResCSV.Open(FLAT_FILE, CFile::modeCreate | CFile::modeWrite))
		{
			return;
		}

		TiXmlElement* root = doc.FirstChildElement();//XML_Sources
		TiXmlElement* Nature_root = root->FirstChildElement()->NextSiblingElement()->FirstChildElement();//Première Nature
		//Une ligne CSV par Child de l'élément envoyé à Recursion et de ceux de ses Siblings
		Recursion(Nature_root, &fileResCSV);
	}

	fileResCSV.Close();
	return;
}