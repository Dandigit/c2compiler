/* Copyright 2013 Bas van den Berg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CCODE_GENERATOR_H
#define CCODE_GENERATOR_H

#include <string>
#include <vector>
#include "StringBuilder.h"
#include "Package.h"

namespace C2 {

class C2Sema;
class Decl;
class Expr;
class DeclExpr;
class Type;
class FunctionDecl;
class Stmt;
class CompoundStmt;

// generates LLVM Module from (multiple) ASTs
class CCodeGenerator {
public:
    enum Mode { MULTI_FILE, SINGLE_FILE };
    CCodeGenerator(const std::string& filename_, Mode mode_, const Pkgs& pkgs_);
    ~CCodeGenerator();
    void addEntry(const std::string& filename, C2Sema& sema);

    void generate();
    void write(const std::string& target, const std::string& name);
    void dump();

private:
    const char* ConvertType(C2::Type* type);

    void EmitFunction(Decl* D);
    void EmitVariable(Decl* D);
    void EmitType(Decl* D);
    void EmitUse(Decl* D);

    void EmitStmt(Stmt* S, unsigned indent);
    void EmitCompoundStmt(CompoundStmt* C, unsigned indent, bool startOnNewLine);
    void EmitIfStmt(Stmt* S, unsigned indent);
    void EmitWhileStmt(Stmt* S, unsigned indent);
    void EmitDoStmt(Stmt* S, unsigned indent);

    void EmitExpr(Expr* E, StringBuilder& output);
    void EmitBinaryOperator(Expr* E, StringBuilder& output);
    void EmitUnaryOperator(Expr* E, StringBuilder& output);
    void EmitMemberExpr(Expr* E, StringBuilder& output);
    void EmitDeclExpr(DeclExpr* D, StringBuilder& output, unsigned indent);
    void EmitCallExpr(Expr* E, StringBuilder& output);
    void EmitIdentifierExpr(Expr* E, StringBuilder& output);

    // Helpers
    void EmitFunctionProto(FunctionDecl* F, StringBuilder& output);
    void EmitTypePreName(Type* T, StringBuilder& output);
    void EmitTypePostName(Type* T, StringBuilder& output);

    const std::string& filename;
    const std::string* curpkg;
    Mode mode;

    const Pkgs& pkgs;

    struct Entry {
        Entry(const std::string& f, C2Sema& s)
            : filename(&f), sema(&s) {}
        const std::string* filename;
        C2Sema* sema;
    };
    typedef std::vector<Entry> Entries;
    typedef Entries::iterator EntriesIter;
    Entries entries;

    StringBuilder cbuf;
    StringBuilder hbuf;
    std::string cfilename;
    std::string hfilename;

    CCodeGenerator(const CCodeGenerator&);
    CCodeGenerator& operator= (const CCodeGenerator&);
};

}

#endif
