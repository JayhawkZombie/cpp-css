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

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

namespace cm = Catch::Matchers;
using namespace css;

/************************************************************************/
/* Type selectors
   Just the generic type selector - no classes or IDs
*/
/************************************************************************/
SCENARIO("Parsing type selectors", "[selector]")
{
  const std::string emptyName = "";

  GIVEN("An input string with a type selector")
  {
    std::stringstream InputString("");

    WHEN("the type is well-formed")
    {
      InputString.str("test-name");
      TypeSelector NParser;
      bool nameParsed = InputString >> NParser;

      THEN("the type can be parsed") 
      { 
        REQUIRE(nameParsed);
      }
      AND_THEN("the type parser stores the name")
      {
        REQUIRE_THAT(NParser.Text, cm::Equals("test-name"));
      }
    }

    WHEN("the type is ill-formed")
    {
      InputString.str("_bad^name");
      TypeSelector NParser;
      bool nameParsed = InputString >> NParser;

      THEN("the type cannot be parsed")
      {
        REQUIRE_FALSE(nameParsed);
      }
      AND_THEN("the type parser remains empty")
      {
        REQUIRE_THAT(NParser.Text, cm::Equals(""));
      }
      AND_THEN("the input is restored to its previous state")
      {
        REQUIRE_THAT(InputString.str(), cm::Equals("_bad^name"));
      }
    }

  } // GIVEN ("An input string with a name selector")
}

/************************************************************************/
/* ID selectors
   Both with a type prefixed and without
*/
/************************************************************************/
SCENARIO("Parsing id selectors", "[id-selectors]")
{
  GIVEN("An input string with an id selector")
  {
    std::stringstream InputString("");

    WHEN("the string begins with a type selector")
    {
      InputString.str("test-name#test-id");
      TypeSelector TSel;
      IDSelector IDSel;

      bool typeParsed = InputString >> TSel;
      bool idParsed = InputString >> IDSel;

      THEN("the type can be parsed")
      {
        REQUIRE(typeParsed);
      }
      THEN("the type selector stores the type")
      {
        REQUIRE_THAT(TSel.Text, cm::Equals("test-name"));
      }
      THEN("the id can be parsed")
      {
        REQUIRE(idParsed);
      }
      THEN("the id selector stores the id")
      {
        REQUIRE_THAT(IDSel.Text, cm::Equals("test-id"));
      }
    }

    WHEN("the string only contains an id selector")
    {
      InputString.str("#test-id");
      IDSelector IDSel;

      bool parsed = InputString >> IDSel;
      THEN("the id can be parsed")
      {
        REQUIRE(parsed);
      }
      THEN("the id selector stores the id")
      {
        REQUIRE_THAT(IDSel.Text, cm::Equals("test-id"));
      }
    }

  } //GIVEN("An input string with an id selector")
}

SCENARIO("Parsing class selectors", "[class-selectors]")
{
  GIVEN("An input string with a class selector")
  {
    std::stringstream InputString("test-name.class-name");

    WHEN("the string begins with a type selector")
    {
      TypeSelector TSel;
      ClassSelector CSel;

      bool typeParsed = InputString >> TSel;
      bool classParsed = InputString >> CSel;

      THEN("the type can be parsed")
      {
        REQUIRE(typeParsed);
      }
      AND_THEN("the type selector stores the type")
      {
        REQUIRE_THAT(TSel.Text, cm::Equals("test-name"));
      }
      AND_THEN("the class can be parsed")
      {
        REQUIRE(classParsed);
      }
      AND_THEN("the class selector stores the class")
      {
        REQUIRE_THAT(CSel.Text, cm::Equals("class-name"));
      }
    }

    AND_WHEN("the string begins with a class selector")
    {
      InputString.str(".class-name");
      ClassSelector CSel;

      bool classParsed = InputString >> CSel;

      THEN("the class can be parsed")
      {
        REQUIRE(classParsed);
      }
      AND_THEN("the class selector stores the class")
      {
        REQUIRE_THAT(CSel.Text, cm::Equals("class-name"));
      }
    }

  } // GIVEN ("An input string with a class selector")
}

SCENARIO("Parsing attribute selectors", "[attribute-selector]")
{
  std::stringstream InputString("");

  GIVEN("An input string with an attribute selector")
  {

    WHEN("the attribute selector is well-formed")
    {
      InputString.str("test-name[attr*=value]");
      TypeSelector TSel;
      AttributeSelector ASel;
    
      bool typeParsed = InputString >> TSel;
      bool attrParsed = InputString >> ASel;

      THEN("The type can be parsed")
      {
        REQUIRE(typeParsed);
      }
      THEN("the type selector stores the type")
      {
        REQUIRE_THAT(TSel.Text, cm::Equals("test-name"));
      }
      THEN("the attribute can be parsed")
      {
        REQUIRE(attrParsed);
      }
      THEN("the attribute selector stores the attribute, the comparison, and the value")
      {
        REQUIRE_THAT(ASel.AttrText, cm::Equals("attr"));
        REQUIRE_THAT(ASel.CompText, cm::Equals("*="));
        REQUIRE_THAT(ASel.ValText,  cm::Equals("value"));
      }
    }

    WHEN("the attribute is ill-formed")
    {
      InputString.str("good-name[attr%(val]");

      TypeSelector TSel;
      AttributeSelector ASel;

      bool typeParsed = InputString >> TSel;
      bool attrParsed = InputString >> ASel;

      THEN("the type can be parsed")
      {
        REQUIRE(typeParsed);
      }
      THEN("the type selector stores the type")
      {
        REQUIRE_THAT(TSel.Text, cm::Equals("good-name"));
      }
      THEN("the attribute cannot be parsed")
      {
        REQUIRE_FALSE(attrParsed);
      }
      THEN("the attribute selector stores nothing")
      {
        REQUIRE_THAT(ASel.AttrText, cm::Equals(""));
        REQUIRE_THAT(ASel.CompText, cm::Equals(""));
        REQUIRE_THAT(ASel.ValText , cm::Equals(""));
      }
      THEN("the input source is restored")
      {
        REQUIRE_THAT(InputString.str(), cm::Equals("good-name[attr%(val]"));
      }
    }
  }
}


SCENARIO("Parsing a list of selectors", "[selector-list]")
{
  std::stringstream InputString("");

  GIVEN("A selector list with multiple selectors")
  {

    InputString.str("test-simple test.with-class test#with-id test[withattr*=value] .lone-class-without-type #lone-id-without-type");

    WHEN("The selector list is well-formed")
    {
      TypeSelector      TypeLone;
      TypeSelector      TypeWithClass;
      ClassSelector     TClass;
      TypeSelector      TypeWithId;
      IDSelector        TId;
      TypeSelector      TypeWithAttr;
      AttributeSelector TAttr;
      ClassSelector     LoneClass;
      IDSelector        LoneID;

      bool loneTypeParsed      = InputString >> TypeLone;
      bool typeWithClassParsed = InputString >> TypeWithClass;
      bool classParsed         = InputString >> TClass;
      bool typeWithIdParsed    = InputString >> TypeWithId;
      bool idParsed            = InputString >> TId;
      bool typeWithAttrParsed  = InputString >> TypeWithAttr;
      bool attrParsed          = InputString >> TAttr;
      bool loneClassParsed     = InputString >> LoneClass;
      bool loneIdParsed        = InputString >> LoneID;

      THEN("a lone type selector can be parsed")
      {
        REQUIRE(loneTypeParsed);
        REQUIRE_THAT(TypeLone.Text, cm::Equals("test-simple"));
      }
      THEN("a type with a class selector can be parsed")
      {
        REQUIRE(typeWithClassParsed);
        REQUIRE_THAT(TypeWithClass.Text, cm::Equals("test"));
        REQUIRE(classParsed);
        REQUIRE_THAT(TClass.Text, cm::Equals("with-class"));
      }
      THEN("a type with an id selector can be parsed")
      {
        REQUIRE(typeWithIdParsed);
        REQUIRE_THAT(TypeWithId.Text, cm::Equals("test"));
        REQUIRE(idParsed);
        REQUIRE_THAT(TId.Text, cm::Equals("with-id"));
      }
      THEN("a type selector with an attribute selector can be parsed")
      {
        REQUIRE(typeWithAttrParsed);
        REQUIRE_THAT(TypeWithAttr.Text, cm::Equals("test"));
        REQUIRE(attrParsed);
        REQUIRE_THAT(TAttr.AttrText, cm::Equals("withattr"));
        REQUIRE_THAT(TAttr.CompText, cm::Equals("*="));
        REQUIRE_THAT(TAttr.ValText , cm::Equals("value"));
      }
      THEN("a lone class selector without a type selector can be parsed")
      {
        REQUIRE(loneClassParsed);
        REQUIRE_THAT(LoneClass.Text, cm::Equals("lone-class-without-type"));
      }
      THEN("a lone id selector without a type selector can be parsed")
      {
        REQUIRE(loneIdParsed);
        REQUIRE_THAT(LoneID.Text, cm::Equals("lone-id-without-type"));
      }
    }

  }

}

SCENARIO("Parsing declarations in a block of style rules", "[style-declaration]")
{
  std::stringstream InputString("");

  GIVEN("a style rule in a declaration block")
  {

    WHEN("the declaration is well-formed")
    {
      InputString.str("property: value42;");

      Declaration decl;
      bool declParsed = InputString >> decl;

      THEN("the declaration cna be parsed")
      {
        REQUIRE(declParsed);
      }
      THEN("the declaration parser stores the property")
      {
        REQUIRE_THAT(decl.PropertyText, cm::Equals("property"));
      }
      THEN("the declaration parser stores the value")
      {
        REQUIRE_THAT(decl.ValueText, cm::Equals("value42"));
      }

    }

    WHEN("the declaration is ill-formed")
    {
      InputString.str("prop= val_?");

      Declaration decl;
      bool declParsed = InputString >> decl;

      THEN("the declaration cannot be parsed")
      {
        REQUIRE_FALSE(declParsed);
      }
      THEN("the declaration parser does not store a property")
      {
        REQUIRE_THAT(decl.PropertyText, cm::Equals(""));
      }
      THEN("the declaration parser does not store a value")
      {
        REQUIRE_THAT(decl.ValueText, cm::Equals(""));
      }
    }
  }

  GIVEN("multiple 3 rules are in a declaration block")
  {

    WHEN("all rules are well-formed")
    {
      InputString.str(R"(simple-prop: value42; 
                         another: good-value; 
                         yet-another: rgba(0, 1, 1, 1);)");

      THEN("all rules will parse")
      {
        Declaration decls[3];
        bool declsParsed[3];

        declsParsed[0] = InputString >> decls[0];
        declsParsed[1] = InputString >> decls[1];
        declsParsed[2] = InputString >> decls[2];

        REQUIRE(declsParsed[0]);
        REQUIRE(declsParsed[1]);
        REQUIRE(declsParsed[2]);

        REQUIRE_THAT(decls[0].PropertyText, cm::Equals("simple-prop"));
        REQUIRE_THAT(decls[0].ValueText,    cm::Equals("value42"));
        
        REQUIRE_THAT(decls[1].PropertyText, cm::Equals("another"));
        REQUIRE_THAT(decls[1].ValueText,    cm::Equals("good-value"));

        REQUIRE_THAT(decls[2].PropertyText, cm::Equals("yet-another"));
        REQUIRE_THAT(decls[2].ValueText,    cm::Equals("rgba(0, 1, 1, 1)"));
      }
    }

  }

}

SCENARIO("Parsing an enclosed declaration block", "[enc-decl-block]")
{
  std::stringstream InputString("");

  GIVEN("An input string with a declaration block enclosed in {}")
  {
    
    WHEN("all rules in the block are well-formed")
    {
      InputString.str(R"({ simple-prop: value42; 
                           another: good-value; 
                           yet-another: rgba(0, 1, 1, 1);
                         })");

      THEN("all rules will parse")
      {
        bool blockParsed;
        DeclarationBlock DBlock;

        blockParsed = InputString >> DBlock;
        REQUIRE(blockParsed);
        REQUIRE(DBlock.Rules.size() == 3);
      }
    }

    WHEN("not all rules in the block are well-formed")
    {
      InputString.str(R"({ simple-prop: value42; 
                           another: good-value; 
                           yet-another: rgba(0, 1, 1, 1);
                           goodname =badval^
                         })");
      THEN("valid rules will still parse and invalid ones will not")
      {
        bool blockparsed;
        DeclarationBlock DBlock;

        blockparsed = InputString >> DBlock;
        REQUIRE(blockparsed);
        REQUIRE(DBlock.Rules.size() == 3);
      }
    }
  }
}

SCENARIO("Parsing a complete style block with selector list", "[selectors-block]")
{
  std::stringstream InputString("");

  GIVEN("an input string with a selector list and declaration block")
  {

    WHEN("the selector list and declaration block are well-formed")
    {
      InputString.str(R"(test-simple test.with-class test#with-id test[withattr*=value] .lone-class-without-type #lone-id-without-type 
                         { simple-prop: value42; 
                           another: good-value; 
                           yet-another: rgba(0, 1, 1, 1);
                         })");

      THEN("the selector list and declaration block will parse")
      {
        TypeSelector      TypeLone;
        TypeSelector      TypeWithClass;
        ClassSelector     TClass;
        TypeSelector      TypeWithId;
        IDSelector        TId;
        TypeSelector      TypeWithAttr;
        AttributeSelector TAttr;
        ClassSelector     LoneClass;
        IDSelector        LoneID;

        bool loneTypeParsed      = InputString >> TypeLone;
        bool typeWithClassParsed = InputString >> TypeWithClass;
        bool classParsed         = InputString >> TClass;
        bool typeWithIdParsed    = InputString >> TypeWithId;
        bool idParsed            = InputString >> TId;
        bool typeWithAttrParsed  = InputString >> TypeWithAttr;
        bool attrParsed          = InputString >> TAttr;
        bool loneClassParsed     = InputString >> LoneClass;
        bool loneIdParsed        = InputString >> LoneID;

        DeclarationBlock DBlock;
        bool blockParsed = InputString >> DBlock;

        REQUIRE(loneTypeParsed);
        REQUIRE(typeWithClassParsed);
        REQUIRE(classParsed);
        REQUIRE(typeWithIdParsed);
        REQUIRE(idParsed);
        REQUIRE(typeWithAttrParsed);
        REQUIRE(attrParsed);
        REQUIRE(loneClassParsed);
        REQUIRE(loneIdParsed);

        REQUIRE_THAT(TypeLone.Text,      cm::Equals("test-simple"));
        REQUIRE_THAT(TypeWithClass.Text, cm::Equals("test"));
        REQUIRE_THAT(TClass.Text,        cm::Equals("with-class"));
        REQUIRE_THAT(TypeWithId.Text,    cm::Equals("test"));
        REQUIRE_THAT(TId.Text,           cm::Equals("with-id"));
        REQUIRE_THAT(TypeWithAttr.Text,  cm::Equals("test"));
        REQUIRE_THAT(TAttr.AttrText,     cm::Equals("withattr"));
        REQUIRE_THAT(TAttr.CompText,     cm::Equals("*="));
        REQUIRE_THAT(TAttr.ValText,      cm::Equals("value"));
        REQUIRE_THAT(LoneClass.Text,     cm::Equals("lone-class-without-type"));
        REQUIRE_THAT(LoneID.Text,        cm::Equals("lone-id-without-type"));

        REQUIRE(blockParsed);
        REQUIRE(DBlock.Rules.size() == 3);
        REQUIRE_THAT(DBlock.Rules[0].PropertyText, cm::Equals("simple-prop"));
        REQUIRE_THAT(DBlock.Rules[0].ValueText,    cm::Equals("value42"));

        REQUIRE_THAT(DBlock.Rules[1].PropertyText, cm::Equals("another"));
        REQUIRE_THAT(DBlock.Rules[1].ValueText,    cm::Equals("good-value"));

        REQUIRE_THAT(DBlock.Rules[2].PropertyText, cm::Equals("yet-another"));
        REQUIRE_THAT(DBlock.Rules[2].ValueText,    cm::Equals("rgba(0, 1, 1, 1)"));
      }

    }

  }
}