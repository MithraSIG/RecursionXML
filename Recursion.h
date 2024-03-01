#pragma once
#include "tinyxml.h"
#include<afx.h>
#include <stdio.h>
#include <iostream>
#include<string>

void Recursion_(TiXmlElement* node, CStdioFile*);
void Recursion(TiXmlElement* node, CStdioFile*, bool);

