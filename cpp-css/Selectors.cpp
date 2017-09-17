////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright(c) 2017 Kurt Slagle - kurt_slagle@yahoo.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The origin of this software must not be misrepresented; you must not claim
// that you wrote the original software.If you use this software in a product,
// an acknowledgment of the software used is required.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Internal Headers
////////////////////////////////////////////////////////////
#include <Selectors.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace css
{

  inline bool operator>>(std::istream &Input, StringParser &Parser)
  {
    if (!Input)
      return false;
    IgnoreWhitespace(Input);

    if (!Input.peek() || !isalpha(Input.peek()))
      return false;

    std::string Temp = "";
    while (Input && isalpha(Input.peek()))
      Temp += Input.get();

    Parser.Text = Temp;
    return true;
  }

  /************************************************************************/
  /* Type selector                                                        */
  /************************************************************************/
  bool TypeSelector::ParseFromInput(std::istream &Input)
  {
    if (!Input)
      REPORT_IO_SOURCE_INVALID_AND_RETURN(false);

    /* ID elements must start with an alpha, then can be any alpha, number, or '-' */
    IgnoreWhitespace(Input);

    std::string Temp = "";

    if (!isalpha(Input.peek())) {
      return false;
    }

    Temp += ( char )Input.get();
    char c = ( char )Input.peek();

    while (Input && ( isalnum(c) || c == '-' )) {
      Temp += ( char )Input.get();
      c = ( char )Input.peek();
    }

    Text = Temp;
    return true;
  }

  /************************************************************************/
  /* Class selector                                                       */
  /************************************************************************/
  bool ClassSelector::ParseFromInput(std::istream &Input)
  {
    if (!Input)
      REPORT_IO_SOURCE_INVALID_AND_RETURN(false);

    IgnoreWhitespace(Input);

    if (( char )Input.peek() != '.')
      return false;

    Input.ignore();

    TypeSelector ClassName;
    if (!( Input >> ClassName )) {
      Input.putback('.');
      return false;
      return false;
    }

    Text = ClassName.Text;
    return true;
  }

  /************************************************************************/
  /* ID Selector                                                          */
  /************************************************************************/
  bool IDSelector::ParseFromInput(std::istream &Input)
  {
    if (!Input)
      REPORT_IO_SOURCE_INVALID_AND_RETURN(false);

    IgnoreWhitespace(Input);

    if (( char )Input.peek() != '#')
      return false;

    Input.ignore();

    TypeSelector IDName;
    if (!( Input >> IDName )) {
      Input.putback('#');
      return false;
      return false;
    }

    Text = IDName.Text;
    return true;
  }

  /************************************************************************/
  /* Attribute selector                                                   */
  /************************************************************************/
  bool AttributeSelector::ParseFromInput(std::istream &Input)
  {
    if (!Input)
      REPORT_IO_SOURCE_INVALID_AND_RETURN(false);

    if (Input.peek() != '[')
      return false;

    Input.ignore();

    std::string TempAttr{ "" }, TempComp{ "" }, TempVal{ "" };
    StringParser AttrString, ValString;

    if (!( Input >> AttrString ))
      return false;

    char c = Input.peek();
    while (c == '=' || IsOneOf(c, "|^$~*")) {
      TempComp += Input.get();
      c = Input.peek();
    }

    if (TempComp.empty() || TempComp.size() > 2) {
      UndoExtraction(Input, TempComp);
      UndoExtraction(Input, AttrString.Text);
      return false;
    }

    if (!( Input >> ValString )) {
      UndoExtraction(Input, TempComp);
      UndoExtraction(Input, AttrString.Text);
      return false;
    }

    if (Input.peek() != ']') {
      UndoExtraction(Input, ValString.Text);
      UndoExtraction(Input, TempComp);
      UndoExtraction(Input, AttrString.Text);
      return false;
    }

    Input.ignore();

    AttrText = AttrString.Text;
    CompText = TempComp;
    ValText = ValString.Text;
    return true;
  }

  /************************************************************************/
  /* Declarations                                                         */
  /************************************************************************/
  bool Declaration::ParseFromInput(std::istream &Input)
  {
    if (!Input)
      return false;

    TypeSelector Prop;

    if (!( Input >> Prop )) {
      return false;
    }

    if (( char )Input.peek() != ':') {
      return false;
    }

    Input.ignore();
    IgnoreWhitespace(Input);

    std::string val{ "" };

    std::getline(Input, val, ';');

    PropertyText = Prop.Text;
    ValueText = val;
    return true;
  }

  /************************************************************************/
  /* Declaration block                                                    */
  /************************************************************************/
  bool DeclarationBlock::ParseFromInput(std::istream &Input)  
  {
  if (!Input)
    return false;

  IgnoreWhitespace(Input);

  if (Input.peek() != '{')
    return false;

  Input.ignore();

  while (true) {
    IgnoreWhitespace(Input);

    if (Input.peek() == '}')
      break;

    Declaration decl;
    if (!( Input >> decl ))
      break;

    Rules.push_back(decl);
  }

  //Ignore anything remaining until we either run out of input (return false) or reach the closing brace
  while (Input && Input.peek() != '}')
    Input.ignore();

  if (!Input)
    return false;

  if (Input.peek() == '}')
    Input.ignore();

  return true;
  }

}
