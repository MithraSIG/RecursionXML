#pragma once

#include <string>
#include <regex>
#include <vector>
#include <cctype> // std::isspace
#include <algorithm> //std::find, std::find_if
#include <fstream> // std::ifstream
#include <cmath>
#include <filesystem> // C++ 2017 Gestion des fichiers
#define NOMINMAX // Desactive les min et max de Windows

#if _HAS_CXX17
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif
// - Version 2 de la class Cast indépendante de Windows

namespace gmd
{
	namespace String
	{
		// o convert a complete string to lower case
		inline std::string to_lower(const std::string& str)
		{
			std::string data = str;
			// convert string to back to lower case
			std::for_each(data.begin(), data.end(), [](char & c) {
				c = ::tolower(c);
			});
			return data;
		}

		// o convert a complete string to upper case
		inline std::string to_upper(const std::string& str)
		{
			std::string data = str;
			// convert string to back to lower case
			std::for_each(data.begin(), data.end(), [](char& c) {
				c = ::toupper(c);
				});
			return data;
		}

		// Ne conserve que certains caractères d'une chaine
		inline void spanIncluding(std::string &str, std::string caracsFilter) {
			std::string currentStr = "";

			for (int i = 0; i < str.length(); i++) {
				if (caracsFilter.find(str[i], 0) == std::string::npos) continue;
				currentStr += str[i];
			}
			str = currentStr;
		}

		// - Split très simple depuis std::string
		inline std::vector<std::string> split(const char* str, char c)
		{
			std::vector<std::string> result;
			do
			{
				const char* begin = str;

				while (*str != c && *str)
					str++;

				result.push_back(std::string(begin, str));
			} while (0 != *str++);

			return result;
		}

		inline std::vector<std::string> split(std::string str, char c)
		{
			return split(str.c_str(), c);
		}

		// - Split très simple depuis std::string
		inline std::vector<std::string> split(std::string str, std::string sep)
		{
			if (sep.length() == 1)
				return split(str.c_str(), sep[0]);

			std::vector<std::string> result;
			bool isSep = false;
			std::string currentStr = "";

			if (sep.empty())
			{
				result.push_back(str);
				return result;
			}

			for (size_t i = 0; i < str.length(); i++) {
				if (str[i] == sep[0]) {
					// Test des autres caractères
					isSep = true;
					for (int k = 1; k < sep.length(); k++) {
						if (i + k >= str.length()) { // Fin de chaine
							isSep = false; break;
						}
						else if (str[i + k] != sep[k]) { // Non concordance des caractères suivants
							isSep = false; break;
						}
					}
					if (isSep) 
					{
						i += sep.length() - 1; // Passage de l'index après le séparateur
						//jb 2022/07/13 on peut vouloir splitter une valeur vide
						// if (currentStr != "") // Ajout de la chaine courante si non vide :
						result.push_back(currentStr); 
						currentStr = ""; // Chaine courante à vide
					}
					else
						currentStr += str[i];
				}
				else
					currentStr += str[i];
			}
			//jb 2022/07/13 on peut vouloir splitter une valeur vide
			//if (currentStr != "") // Ajout de la chaine courante si non vide
			result.push_back(currentStr);
			
			return result;
		}

		inline std::vector<double> splitDouble(std::string str, char c)
		{
			std::vector<std::string> vs = split(str.c_str(), c);
			std::vector<double> vd;

			for (size_t i = 0; i < vs.size(); i++) {
				if (vs[i] != "")
					vd.push_back(std::stod(vs[i]));
			}

			vs.clear();
			return vd;
		}

		inline std::vector<int> splitInt(std::string str, char c)
		{
			std::vector<std::string> vs = split(str.c_str(), c);
			std::vector<int> vi;

			for (size_t i = 0; i < vs.size(); i++) {
				if (vs[i] != "")
					vi.push_back(std::stoi(vs[i]));
			}

			vs.clear();
			return vi;
		}

		// trim from start (in place)
		static inline void ltrim(std::string& s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !std::isspace(ch);
			}));
		}

		// trim from end (in place)
		static inline void rtrim(std::string& s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch);
			}).base(), s.end());
		}

		// trim from both ends (in place)
		static inline void trim(std::string& s) {
			ltrim(s);
			rtrim(s);
		}

		static inline void StringToWString(std::wstring& ws, const std::string& s)
		{
			ws = std::wstring(s.begin(), s.end());
		}

		// replace a string by another string
		static inline void replace(std::string& str, const std::string& from, const std::string& to)
		{
			if (from.empty()) return;

			// replace a string by another string
			size_t start_pos = 0;
			while (true) {
				/* Locate the substring to replace. */
				start_pos = str.find(from, start_pos);
				if (start_pos == std::string::npos) break;

				/* Make the replacement. */
				str.replace(start_pos, from.length(), to);

				/* Advance index forward so the next iteration doesn't pick it up as well. */
				start_pos += to.length();
			}
		}

		// JB de nombreuses limites à cette fonction (mais pour des string FRA ca suffit)
		inline std::string _iso_8859_1_to_utf8(const std::string& str)
		{
			std::string strOut;
			for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
			{
				uint8_t ch = *it;
				if (ch < 0x80) {
					strOut.push_back(ch);
				}
				else {
					strOut.push_back(0xc0 | ch >> 6);
					strOut.push_back(0x80 | (ch & 0x3f));
				}
			}
			return strOut;
		}

		inline std::string _utf8_to_iso_8859_1(const std::string& str)
		{
			const char* in = str.c_str();
			std::string out;
			if (in == NULL)
				return out;

			unsigned int codepoint = 0;
			while (*in != 0)
			{
				unsigned char ch = static_cast<unsigned char>(*in);
				if (ch <= 0x7f)
					codepoint = ch;
				else if (ch <= 0xbf)
					codepoint = (codepoint << 6) | (ch & 0x3f);
				else if (ch <= 0xdf)
					codepoint = ch & 0x1f;
				else if (ch <= 0xef)
					codepoint = ch & 0x0f;
				else
					codepoint = ch & 0x07;
				++in;
				if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
				{
					if (codepoint <= 255)
					{
						out.append(1, static_cast<char>(codepoint));
					}
					else
					{
						// do whatever you want for out-of-bounds characters
					}
				}
			}
			return out;
		}
	}
	
	namespace Cast
	{
		static double __aMul[16] = { 1.0,10.0,100.0,1000.0,10000.0,100000.0,1000000.0,10000000.0,100000000.0,1000000000.0,10000000000.0,100000000000.0,1000000000000.0,10000000000000.0,100000000000000.0,1000000000000000.0 };

		/*! \brief arrondi d'un double
		*	\param n'importe quelle valeur de type double
		*	\param le nombre de chiffre de l'arrondi
		*	\return La valeur en double */
		inline double round(const double& v, const int& nbdec)
		{
			const double a = __aMul[nbdec];
			return floor(v * a + 0.5) / a;
		}

		// Conversion string => int, float, double
		inline int toInt(const std::string& s)  { return std::stoi(s); }
		inline float toFloat(const std::string& s)  { return std::stof(s); }
		inline double toDouble(const std::string& s)  { return std::stod(s); }
		inline int toInt(const char* s) { return std::stoi(s); }
		inline float toFloat(const char* s) { return std::stof(s); }
		inline double toDouble(const char* s) { return std::stod(s); }

		// Test type
		inline bool isInt(const std::string& s) {
			try { toInt(s); return true; }
			catch (const std::logic_error& e) { (void)e;  return false;  } //mark the object as "used" by casting it to void. It has no influence on the generated machine code, but it will suppress the compiler warning.
		}
		inline bool isIntStrict(const std::string& s) {
			try { toInt(s); return std::regex_match(s, std::regex("[0-9]+")); }
			catch (const std::logic_error& e) { (void)e;  return false; } //mark the object as "used" by casting it to void. It has no influence on the generated machine code, but it will suppress the compiler warning.
		}
		inline bool isFlt(const std::string& s) {
			try { toFloat(s); return true; }
			catch (const std::logic_error& e) { (void)e;  return false; } 
		}
		inline bool isFiniNumber(const double &i) {
			if (!(_isnan(i)))
				if (_finite(i))
					return true;
			return false;
		}

		// Conversion int, float, double => string
		inline std::string toString(const size_t i) { return std::to_string(i); }
		inline std::string toString(const int i) { return std::to_string(i); }
		inline std::string toString(const unsigned int i) { return std::to_string(i); }
		inline std::string toString(const float f) { return std::to_string(f); }
		inline std::string toString(const double d) { return std::to_string(d); }

		inline std::wstring toWString(const std::string& s) { std::wstring wsTmp(s.begin(), s.end()); return wsTmp; }

		// Conversion float, double => string avec arrondi
		inline std::string toString(const float f, int nbdec) {
			std::string r = std::to_string(f);
			r = r.substr(0, r.find(".") + nbdec + 1);
			if (r[r.length() - 1] == '.') r = r.substr(0, r.length() - 1); // Suppression du caractère '.' si c'est le dernier de la chaîne
			return r;
		}
		inline std::string toString(const double d, int nbdec) {
			std::string r = std::to_string(d);
			r = r.substr(0, r.find(".") + nbdec + 1);
			if (r[r.length() - 1] == '.') r = r.substr(0, r.length() - 1); // Suppression du caractère '.' si c'est le dernier de la chaîne
			return r;
		}
	}

	namespace File
	{
		/*
		 * En C++ 2017/2019, l'utisation des fichiers est simplifiées via cette classe
		* https://en.cppreference.com/w/cpp/header/filesystem
		*/

		/*< \brief test si un fichier existe ou non */
		inline bool __is_readable(const std::string& file)
		{
			std::ifstream fichier(file);
			printf("%s : %s\n", fichier.fail() ? "fail" : "good", file.c_str());
			return !fichier.fail();
		}


		/*! 
		 * vérifie si un fichier peut être relu 
		 * test de l'existance et des droits d'acces
		 *\return True if the file exist and the owner, group and others have read permission,
		 *	i.e. at least 0444.
		 */
		inline bool CanRead(const std::string& file)
		{
			if (  file.size() == 0) return false;

			std::error_code ec; // For noexcept overload usage.
			if (fs::exists(file, ec) && !ec) //Exist
			{
				auto perms = fs::status(file, ec).permissions();
				if ((perms & fs::perms::owner_read) != fs::perms::none &&
					(perms & fs::perms::group_read) != fs::perms::none &&
					(perms & fs::perms::others_read) != fs::perms::none
					)
				{
					return true;
				}
			}
			// printf("Not Readable : %s\n", file.c_str());
			return false;
		}

		/*< \Extension fichier */
		inline std::string getExtension(const std::string& file_path_name)
		{
			const size_t lastdot = file_path_name.find_last_of(".");
			if (lastdot == std::string::npos) return "";
			else			
				return String::to_lower(file_path_name.substr(lastdot + 1, file_path_name.size()));
			return "";
		}

		/*< \brief donne le nom de dossier dnas lequel depuis un nom complet de fichier
		 *  \param un champ text std::string
		 */
		inline	std::string get_folder_path(std::string file)
		{
			const size_t found = file.find_last_of("/\\");
			if (std::string::npos != found)
			{
				return file.substr(0, found);
			}
			return "";
		}
		
		/*< \brief donne le nom d'un fichier depuis un passe complet Windows */
		inline std::string getFilename_FromPath(std::string fullfilename)
		{
			const size_t found = fullfilename.find_last_of("/\\");
			if (found == std::string::npos) return fullfilename;
			return fullfilename.substr(found + 1);
		}

		/*< \brief donne le nom d'un fichier sans extention depuis un passe complet Windows */
		inline std::string getFilename_WithoutExtention(std::string fullfilename)
		{
			std::string filename = getFilename_FromPath(fullfilename);
			const size_t lastdot = filename.find_last_of(".");
			if (lastdot == std::string::npos) return filename;
			return filename.substr(0, lastdot);
		}

		inline void removeFile(const std::string& file_to_remove)
		{
			if (CanRead(file_to_remove.c_str()))
				std::remove(file_to_remove.c_str());
		}	

		/*<
		 * \brief recherche et renvoi tous les fichier correspondant à une critère de recherche
		 * \param path : le nom du dossier ou on effectue la rechercher (/path/to/directory ou d:/directory)
		 * \param include_subfolders : true si on souhaite rechercher dans les sous-dossiers
		 */
		inline std::vector<std::string> search_directory(std::string path, bool include_subfolders = false)
		{
			// si on se trouve dans le répertoire courant le path doit être "." pour eviter une ex
			std::string my_pass = (path.size()) ? path : ".";

			std::error_code ec; // For noexcept overload usage.
			std::vector<std::string> dir;
			if (fs::exists(my_pass, ec) && !ec) //Check if directory exist
			{
				for (const auto& entry : fs::directory_iterator(my_pass, ec))
				{
					if (fs::is_directory(entry.path().string())) {
						dir.push_back(entry.path().string());
						if (include_subfolders) {
							std::vector<std::string> dir2 = search_directory(entry.path().string(), include_subfolders);
							dir.insert(dir.end(), dir2.begin(), dir2.end());
						}
					}
				}
			}

			return dir;
		}

		/*<
		 * \brief recherche et renvoi tous les fichier correspondant à une critère de recherche
		 * \param path : le nom du dossier ou on effectue la rechercher (/path/to/directory ou d:/directory)
		 * \param extension : le filtre critère de recherche filtre (.res). Bien mettre le point
		 * \param include_subfolders : true si on souhaite rechercher dans les sous-dossiers
		 */
		inline std::vector<std::string> search_file(std::string path, const char* extension, bool include_subfolders = false)
		{
			// printf("search_file PATH : %s\n", path.c_str());
			// si on se trouve dans le répertoire courant le path doit être "." pour eviter une ex
			std::string my_pass = (path.size()) ? path : "."; 
			std::string ext = "";

			if (extension)
			{
				ext = extension;
				if (ext.size() > 1)
				{
					// Si le "*" est le premier char on le supprime
					if (ext.front() == '*')ext.erase(0, 1);
					// Si le "." n'est pas le premier charactère on le rajoute
					if (ext.front() != '.')ext = '.' + ext;
				}
			}

			std::error_code ec; // For noexcept overload usage.
			std::vector<std::string> files;
			const bool isNULL = extension == NULL;
			if (fs::exists(my_pass, ec) && !ec) //Check if directory exist
			{
				for (const auto& entry : fs::directory_iterator(my_pass, ec))
				{
					// printf("\t -  %s\n", entry.path().string().c_str());
					if (!fs::is_directory(entry.path().string())) {
						if (isNULL || entry.path().extension() == ext)
							files.push_back(entry.path().string());
					}
					else if (include_subfolders) {
						std::vector<std::string> files2 = search_file(entry.path().string(), extension, include_subfolders);
						files.insert(files.end(), files2.begin(), files2.end());
					}
				}
			}

			return files;
		}
		/*<
		* \brief recherche et renvoi tous les fichier correspondant à une critère de recherche
		* \param path : le nom du dossier ou on effectue la rechercher (/path/to/directory ou d:/directory)
		* \param str : la chaine de caractères que doit contenir le nom complet du fichier
		* \param include_subfolders : true si on souhaite rechercher dans les sous-dossiers
		*/
		inline std::vector<std::string> search_file_withFilter(std::string path, std::string str, bool include_subfolders = false)
		{
			// printf("search_file_withFilter PATH : %s\n", path.c_str());
			// si on se trouve dans le répertoire courant le path doit être "." pour eviter une ex
			std::string my_pass = (path.size()) ? path : ".";
			const bool isNULL = str == "";
			std::vector<std::string> files;
			for (const auto& entry : fs::directory_iterator(my_pass))
			{
				// printf("\t -  %s\n", entry.path().string().c_str());
				if (!fs::is_directory(entry.path().string())) {
					if (isNULL || entry.path().string().find(str) != std::string::npos)
						files.push_back(entry.path().string());
				}
				else if (include_subfolders) {
					std::vector<std::string> files2 = search_file_withFilter(entry.path().string(), str, include_subfolders);
					files.insert(files.end(), files2.begin(), files2.end());
				}
			}
			return files;
		}

		/*< \brief Creates the directory p as if by POSIX mkdir()
		*  https://en.cppreference.com/w/cpp/filesystem/create_directory
		* (the parent directory must already exist)
		*/
		inline bool createDirectory(const fs::path& p)
		{
			std::error_code ec; // For noexcept overload usage.
			if (!fs::create_directories(p, ec))
			{
				if (ec.value() > 0)
				{
					printf("Error[createDirectory] : %s\n", ec.message().c_str());
					return false;
				}
				else
				{
					return true;
				}
					
			}
			return true;
		}

		/*< \brief copie d'un fichier
		 * \param le nom du fichier à copier
		 * \param le nom du fichier qui va être copié
		 * https://en.cppreference.com/w/cpp/filesystem/copy_file
		*/
		inline void copyFile(std::string file_in, std::string file_out, fs::copy_options options = fs::copy_options::none)
		{
			fs::copy_file(file_in, file_out, options);
		}
		
		inline void copyFileOverwrite(std::string file_in, std::string file_out)
		{
			copyFile(file_in, file_out, fs::copy_options::overwrite_existing);
		}

		inline void moveFile(std::string file_in, std::string file_out)
		{
			fs::rename(file_in, file_out);
		}

		inline void removeDirectory(const std::string& directory_to_remove)
		{
			std::error_code er;
			if (fs::remove_all(directory_to_remove, er) == -1) {
				fs::path directory = directory_to_remove;
				for (auto& path : fs::recursive_directory_iterator(directory))
				{
					try {
						fs::permissions(path, fs::perms::all); // Uses fs::perm_options::replace.
					}
					catch (std::exception& e) {
						// Handle exception or use another overload of fs::permissions()
						// with std::error_code.
					}
				}
				fs::remove_all(directory_to_remove, er);
			}
		}

		inline bool isDirectoryExists(const std::string& directory)
		{
			return fs::is_directory(directory);
		}
	}
}