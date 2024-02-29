# include "Recursion.h"

void Recursion(TiXmlElement* node)
{

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
		CString Csv_Source_Info = "";
		TiXmlElement* root = doc.FirstChildElement();//XML_Sources
		TiXmlElement* Nature_root = root->FirstChildElement()->NextSiblingElement()->FirstChildElement();//Première Nature

		while (Nature_root)
		{
			TiXmlElement* Source_root = Nature_root->FirstChildElement();//1ere Source
			while (Source_root)
			{
				Csv_Source_Info = Csv_Source_Info + Source_root->Value() + "\n";
				fileResCSV.WriteString(Csv_Source_Info);
				Csv_Source_Info = "";
				Source_root = Source_root->NextSiblingElement();
			}

			//On passe à la nature suivante
			Nature_root = Nature_root->NextSiblingElement();

		}
	}
	fileResCSV.Close();
	return;
}