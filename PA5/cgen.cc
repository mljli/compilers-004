
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
       arg,
       arg2,
       Bool,
       concat,
       cool_abort,
       copy_meth,
       Int,
       in_int,
       in_string,
       IO,
       length,
       Main,
       main_meth,
       No_class,
       No_type,
       Object,
       out_int,
       out_string,
       prim_slot,
       self,
       SELF_TYPE,
       Str,
       str_field,
       substr,
       type_name,
       val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
  arg         = idtable.add_string("arg");
  arg2        = idtable.add_string("arg2");
  Bool        = idtable.add_string("Bool");
  concat      = idtable.add_string("concat");
  cool_abort  = idtable.add_string("abort");
  copy_meth   = idtable.add_string("copy");
  Int         = idtable.add_string("Int");
  in_int      = idtable.add_string("in_int");
  in_string   = idtable.add_string("in_string");
  IO          = idtable.add_string("IO");
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any 
//   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
  Object      = idtable.add_string("Object");
  out_int     = idtable.add_string("out_int");
  out_string  = idtable.add_string("out_string");
  prim_slot   = idtable.add_string("_prim_slot");
  self        = idtable.add_string("self");
  SELF_TYPE   = idtable.add_string("SELF_TYPE");
  Str         = idtable.add_string("String");
  str_field   = idtable.add_string("_str_field");
  substr      = idtable.add_string("substr");
  type_name   = idtable.add_string("type_name");
  val         = idtable.add_string("_val");
}

static char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream& s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
    << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << endl; }

static void emit_load_address(char *dest_reg, char *address, ostream& s)
{ s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

static void emit_div(char *dest, char *src1, char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src1, char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

static void emit_sll(char *dest, char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

static void emit_jalr(char *dest, ostream& s)
{ s << JALR << "\t" << dest << endl; }

static void emit_jal(char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l,s);
  s << LABEL;
}

static void emit_beqz(char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_branch(int l, ostream& s)
{
  s << BRANCH;
  emit_label_ref(l,s);
  s << endl;
}

static void emit_jal_method(Symbol classname, Symbol methodname, ostream& s) {
    s << JAL;
    emit_method_ref(classname, methodname, s);
    s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char *reg, ostream& str)
{
  emit_store(reg,0,SP,str);
  emit_addiu(SP,SP,-4,str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }


static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s); // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP,SP,4,s);
  emit_load(ACC,0,SP,s);
}

static void emit_gc_check(char *source, ostream &s)
{
  if (source != (char*)A1) emit_move(A1, source, s);
  s << JAL << "_gc_check" << endl;
}

//
//
static void emit_class_names(ostream& s, CgenNodeP node);
static void emit_class_object_table(ostream& s, CgenNodeP node);
static void emit_dispatch_table(ostream& s, env_type& e, const std::vector<method_classname_pair>& ims);
static void emit_prototype_objects(ostream& s, env_type& e, const std::vector<Feature>& ias);
static void gather_depth(env_type& e);
static void emit_object_initializers(ostream& s, env_type& e);
static void emit_class_methods(ostream& s, env_type& e);

static void emit_precedure_set_up_code(ostream& s);
static void emit_precedure_set_up_code(ostream& s, int depth);
static void emit_precedure_clean_up_code(ostream& s);
static void emit_precedure_clean_up_code(ostream& s, int depth);
static void emit_precedure_clean_up_code(ostream& s, int depth, int narg);
//

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s  << LABEL                                             // label
      << WORD << stringclasstag << endl                                 // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
      << WORD;


 /***** Add dispatch information for class String ******/
  emit_disptable_ref(Str, s);
  s << endl;                                                  // dispatch table
  s << WORD;  lensym->code_ref(s);  s << endl;                // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                // label
      << WORD << intclasstag << endl                      // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
      << WORD; 

 /***** Add dispatch information for class Int ******/
      emit_disptable_ref(Int, s);
      s << endl;                                          // dispatch table
      s << WORD << str << endl;                           // integer value
}


//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,intclasstag);
}


//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
  s << BOOLCONST_PREFIX << val;
}
  
//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                  // label
      << WORD << boolclasstag << endl                       // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
      << WORD;

 /***** Add dispatch information for class Bool ******/

      emit_disptable_ref(Bool, s);
      s << endl;                                            // dispatch table
      s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n" << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
  str << GLOBAL; falsebool.code_ref(str);  str << endl;
  str << GLOBAL; truebool.code_ref(str);   str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;

  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << stringclasstag << endl;    
}


//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL 
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}


//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str,stringclasstag);
  inttable.code_string_table(str,intclasstag);
  code_bools(boolclasstag);
}


CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
   enterscope();
   if (cgen_debug) cout << "Building CgenClassTable" << endl;
   install_basic_classes();
   install_classes(classes);
   build_inheritance_tree();

   // we must ensure that the class tag of a class is less
   // than its parent's class tag
   int count = 0;
   set_class_tags(probe(Object), count);

   intclasstag = probe(Int)->get_class_tag();
   stringclasstag = probe(Str)->get_class_tag();
   boolclasstag = probe(Bool)->get_class_tag();

   code();
   exitscope();
}

void CgenClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
  //curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

//
// A few special class names are installed in the lookup table but not
// the class list.  Thus, these classes exist, but are not part of the
// inheritance hierarchy.
// No_class serves as the parent of Object and the other special classes.
// SELF_TYPE is the self class; it cannot be redefined or inherited.
// prim_slot is a class known to the code generator.
//
  addid(No_class,
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
			    Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    Basic,this));
  addid(prim_slot,
	new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
			    Basic,this));

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//
  install_class(
   new CgenNode(
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy_meth, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
   install_class(
    new CgenNode(
     class_(IO, 
            Object,
            append_Features(
            append_Features(
            append_Features(
            single_Features(method(out_string, single_Formals(formal(arg, Str)),
                        SELF_TYPE, no_expr())),
            single_Features(method(out_int, single_Formals(formal(arg, Int)),
                        SELF_TYPE, no_expr()))),
            single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
            single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	   filename),	    
    Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//
   install_class(
    new CgenNode(
     class_(Int, 
	    Object,
            single_Features(attr(val, prim_slot, no_expr())),
	    filename),
     Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
     new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
      Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  ???
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//       
   install_class(
    new CgenNode(
      class_(Str, 
	     Object,
             append_Features(
             append_Features(
             append_Features(
             append_Features(
             single_Features(attr(val, Int, no_expr())),
            single_Features(attr(str_field, prim_slot, no_expr()))),
            single_Features(method(length, nil_Formals(), Int, no_expr()))),
            single_Features(method(concat, 
				   single_Formals(formal(arg, Str)),
				   Str, 
				   no_expr()))),
	    single_Features(method(substr, 
				   append_Formals(single_Formals(formal(arg, Int)), 
						  single_Formals(formal(arg2, Int))),
				   Str, 
				   no_expr()))),
	     filename),
        Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
    {
      return;
    }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd,nds);
  addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
  for(List<CgenNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}



void CgenClassTable::code()
{
  if (cgen_debug) cout << "coding global data" << endl;
  code_global_data();

  if (cgen_debug) cout << "choosing gc" << endl;
  code_select_gc();

  if (cgen_debug) cout << "coding constants" << endl;
  code_constants();

//                 Add your code to emit
//                   - prototype objects
//                   - class_nameTab
//                   - dispatch tables
//

  // walk through the inheritance graph and emit prototype objects etc.
  env_type e;
  e.classtable = this;
  
  // class names
  str <<  CLASSNAMETAB << LABEL;
  emit_class_names(str, probe(Object));

  // class object table
  str << CLASSOBJTAB << LABEL;
  emit_class_object_table(str, probe(Object));

  // dispatch table
  e.curr_class = probe(Object);
  emit_dispatch_table(str, e, std::vector<method_classname_pair>());

  // prototype objects
  emit_prototype_objects(str, e, std::vector<Feature>());

  if (cgen_debug) cout << "coding global text" << endl;
  code_global_text();

//                 Add your code to emit
//                   - object initializer
//                   - the class methods
//                   - etc...

  // TODO integrate initializer emission with method emission

  // gather nested depths of `let' and `case' for each feature
  gather_depth(e);
  
  // default initializers
  emit_object_initializers(str, e);

  // class methods
  emit_class_methods(str, e);
  e.curr_class = 0;
}


CgenNodeP CgenClassTable::root()
{
   return probe(Object);
}

bool CgenClassTable::is_ancestor(Symbol t1, Symbol t2) {
    CgenNodeP node = probe(t2);
    while (node && node->get_name() != t1) {
        node = node->get_parentnd();
    }
    if (node) {
        return true;
    }
    return false;
}

void CgenClassTable::set_class_tags(CgenNodeP node, int& count) {
    node->set_class_tag(count++);

    for (List<CgenNode>* c = node->get_children(); c; c = c->tl()) {
        set_class_tags(c->hd(), count);
    }
}

///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   children(NULL),
   basic_status(bstatus)
{ 
   stringtable.add_string(name->get_string());          // Add class name to string table
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

void assign_class::code(ostream& s, env_type& e) {
    expr->code(s, e);

    env_type::id_type idtype;
    int offset;
    idtype = e.lookup_object(name, offset);
    switch (idtype) {
    case env_type::ATTRIBUTE:
        emit_store(ACC, offset, SELF, s);
        break;
    case env_type::FORMAL:
        emit_store(ACC, offset, FP, s);
        break;
    case env_type::LOCAL:
        emit_store(ACC, offset, FP, s);
        break;
    case env_type::SELFOBJ:  // semantic analyzer ensures it won't happen
    default:
        assert(0);
    }
}

void static_dispatch_class::code(ostream& s, env_type& e) {
    int label_number = e.blc++;

    // push arguments onto the stack in reverse order
    for (int i = actual->len()-1; i >= 0; --i) {
        actual->nth(i)->code(s, e);
        emit_push(ACC, s);
    }

    expr->code(s, e);
    emit_bne(ACC, ZERO, label_number, s);  // dispatch on void error
    emit_partial_load_address(ACC, s);
    stringtable.lookup_string(e.curr_class->get_filename()->get_string())->code_ref(s);
    s << endl;
    emit_load_imm(T1, 1, s);
    emit_jal(DISPATCH_ON_VOID_ABORT, s);

    emit_label_def(label_number, s);

    emit_push(T1, s);
    emit_push(ACC, s);

    emit_partial_load_address(ACC, s);
    emit_disptable_ref(type_name, s);
    s << endl;
    emit_load(T1, e.method_offset(type_name, name), ACC, s);
    emit_load(ACC, 1, SP, s);
    emit_jalr(T1, s);  // ready to call

    emit_load(T1, 2, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void dispatch_class::code(ostream& s, env_type& e) {
    int label_number = e.blc++;

    expr->code(s, e);
    emit_bne(ACC, ZERO, label_number, s);  // dispatch on void error
    emit_partial_load_address(ACC, s);
    stringtable.lookup_string(e.curr_class->get_filename()->get_string())->code_ref(s);
    s << endl;
    emit_load_imm(T1, 1, s);
    emit_jal(DISPATCH_ON_VOID_ABORT, s);

    emit_label_def(label_number, s);
    for (int i = actual->len()-1; i >= 0; --i) {
        actual->nth(i)->code(s, e);
        emit_push(ACC, s);
    }

    Symbol caller_type = expr->get_type();
    if (caller_type == SELF_TYPE) {
        caller_type = e.curr_class->get_name();
    }
    emit_push(T1, s);

    emit_push(ACC, s);
    emit_partial_load_address(ACC, s);
    emit_disptable_ref(caller_type, s);
    s << endl;
    emit_load(T1, e.method_offset(caller_type, name), ACC, s);
    emit_load(ACC, 1, SP, s);
    emit_jalr(T1, s);  // ready to call

    emit_load(T1, 2, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void cond_class::code(ostream& s, env_type& e) {
    int label_number = e.blc;
    e.blc += 2; // one for else branch, one for the next statement
    pred->code(s, e);

    emit_move(T1, ACC, s);  // value of predicate (t1)
    emit_load_address(T2, BOOLCONST_PREFIX"1", s);  // true (t2)

    emit_load_imm(ACC, 1, s);  // true
    emit_load_imm(ACC, 0, s);  // false

    emit_jal(EQUALITY_TEST, s);
    emit_move(T1, ACC, s);  // equality (t1 == t2)
    emit_beqz(T1, label_number, s);

    then_exp->code(s, e);
    emit_branch(label_number+1, s);

    emit_label_def(label_number, s);
    else_exp->code(s, e);

    emit_label_def(label_number+1, s);
}

void loop_class::code(ostream& s, env_type& e) {
    int label_number = e.blc;
    e.blc += 2;  // one for loop start, one for the next statement

    emit_label_def(label_number, s);
    pred->code(s, e);

    emit_move(T1, ACC, s);  // value of predicate
    emit_load_address(T2, BOOLCONST_PREFIX"0", s);  // true

    emit_load_imm(ACC, 1, s);  // true
    emit_load_imm(ACC, 0, s);  // false

    emit_jal(EQUALITY_TEST, s);
    emit_move(T1, ACC, s);  // equality (t1 == t2)
    emit_beqz(T1, label_number+1, s);

    body->code(s, e);
    emit_branch(label_number, s);

    emit_label_def(label_number+1, s);
    emit_load_imm(ACC, 0, s);  // return `void'
}

// TODO OPT
// Since case statements in COOL operate on types instead of
// values and we know pretty much information about types at
// compile time. We can totally ignore those branches whose 
// identifiers have types that are not on the 'inheritance path'
// of the the type of `expr0'.
//
// We can even filter more useless branches by the method describe
// below (make a marker).
//
// However, this sounds ridiculous because it seems that the compiler
// does too much. I wonder if this should be implemented.
void typcase_class::code(ostream& s, env_type& e) {
    expr->code(s, e);

    int label_number = e.blc, last_label = label_number++;
    // n for n branches
    // one for no matching branch error
    // one for the next statement
    e.blc += 2 + cases->len();

    // push $t2 onto the stack
    emit_push(T2, s);
    emit_load(T2, 0, ACC, s);   // the class tag of `expr0'

    // case on `void'
    emit_bne(ACC, ZERO, label_number, s);
    // see `_case_abort2' for details
    emit_partial_load_address(ACC, s);
    stringtable.lookup_string(e.curr_class->get_filename()->get_string())->code_ref(s);
    s << endl;
    emit_load_imm(T1, 1, s);
    emit_jal(CASE_ON_VOID_ABORT, s);

    // Sort the branches by the type of their declared variables
    // and always generate code for derived classes first.
    // However this requires that the class tag of a class
    // if less than its parent's class tag.
    std::vector<Case> branches;
    for (int i = cases->first(); cases->more(i); i = cases->next(i)) {
        branches.push_back(cases->nth(i));
    }
    // ascending order (class 0, class 1, ...)
    std::sort(branches.begin(), branches.end(), compare_branch_var_type(e.classtable));

    // mark the ending type of matching
    // 
    // For example, if the type of `expr0' is T, mark the nearest
    // ancestor of T that occurs in the candidating branches. If
    // T is SELF_TYPEc, we do same thing by marking the nearest 
    // ancestor of C that it occurs.
    // If none of those ancestors appeared, we don't mark anything.
    // 
    // During matching, if we meet an exactly matched branch before
    // we meet the marked one, we choose that branch. Otherwise, we
    // keep going on until we meet the marked one. In both cases,
    // the branch we choosed is the best match. If neither of these
    // happen, we fallback to a no matching error.
    
    Symbol type_ref = expr->get_type();
    if (type_ref == SELF_TYPE) {
        type_ref = e.curr_class->get_name();
    }
    std::vector<Case>::const_reverse_iterator marker = branches.rend();
    for (std::vector<Case>::const_reverse_iterator rit = branches.rbegin();
            rit != branches.rend(); ++rit) {
        Symbol type_cand = (*rit)->get_type();
        if (e.classtable->is_ancestor(type_cand, type_ref)) {
            marker = rit;
            break;
        }
    }

    for (std::vector<Case>::const_reverse_iterator rit = branches.rbegin();
            rit != branches.rend(); ++rit) {
        emit_label_def(label_number, s);
        int class_tag_cand = e.classtable->probe((*rit)->get_type())->get_class_tag();
        if (rit != marker) {
            emit_bgti(T2, class_tag_cand, label_number+1, s);
            emit_blti(T2, class_tag_cand, label_number+1, s);
        } else {
            emit_blti(T2, class_tag_cand, label_number+1, s);
            // branches after this one will never be selected
        }
        (*rit)->code(s, e);
        ++label_number;
        emit_branch(last_label, s);
    }
    // no matching branch error
    emit_label_def(label_number, s);
    emit_jal(CASE_NO_MATCH_ABORT, s);

    emit_label_def(last_label, s);

    emit_load(T2, 4, SP, s);    // pop $t2
    emit_addiu(SP, SP, -4, s);
}

void branch_class::code(ostream& s, env_type& e) {
    ++e.curr_depth;

    // add identifier to the context
    e.set_local_offset(name);

    // bind a0 to variable `name'
    emit_store(ACC, e.local_offset(name), FP, s);

    expr->code(s, e);

    --e.curr_depth;
    e.pop_the_last_local();
}

void block_class::code(ostream& s, env_type& e) {
    for (int i = body->first(); body->more(i); i = body->next(i)) {
        body->nth(i)->code(s, e);
    }
}

void let_class::code(ostream& s, env_type& e) {
    ++e.curr_depth;

    // initialize the identifier
    Symbol td = type_decl;
    if (td == SELF_TYPE) {
        td = e.curr_class->get_name();
    }

    if (init->get_type() != No_type) {
        emit_partial_load_address(ACC, s);
        emit_protobj_ref(Object, s);
        emit_jal_method(Object, copy_meth, s);
        s << JAL;
        emit_init_ref(td, s);
        s << endl;
    } else {
        init->code(s, e);  // just a single line, sweet:)
    }

    e.set_local_offset(identifier);
    emit_store(ACC, e.local_offset(identifier), FP, s);

    --e.curr_depth;
}

void plus_class::code(ostream& s, env_type& e) {
    e1->code(s, e);
    emit_push(T1, s);
    emit_move(T1, ACC, s);
    e2->code(s, e);
    emit_push(T2, s);
    emit_move(T2, ACC, s);
    emit_add(T1, T1, T2, s);
    // $a0 contains an `Int' object
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_load(T1, 2, SP, s);
    emit_load(T2, 1, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void sub_class::code(ostream& s, env_type& e) {
    e1->code(s, e);
    emit_push(T1, s);
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
    e2->code(s, e);
    emit_push(T2, s);
    emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
    emit_sub(T1, T1, T2, s);
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_load(T1, 2, SP, s);
    emit_load(T2, 1, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void mul_class::code(ostream& s, env_type& e) {
    e1->code(s, e);
    emit_push(T1, s);
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
    e2->code(s, e);
    emit_push(T2, s);
    emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
    emit_mul(T1, T1, T2, s);
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_load(T1, 2, SP, s);
    emit_load(T2, 1, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void divide_class::code(ostream& s, env_type& e) {
    e1->code(s, e);
    emit_push(T1, s);
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
    e2->code(s, e);
    emit_push(T2, s);
    emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
    emit_div(T1, T1, T2, s);
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_load(T1, 2, SP, s);
    emit_load(T2, 1, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void neg_class::code(ostream& s, env_type& e) {
    e1->code(s, e);
    emit_push(T1, s);
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_neg(T1, T1, s);
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, -4, s);
}

void lt_class::code(ostream& s, env_type& e) {
    int label_number = e.blc;
    e.blc += 2;  // one for false, one for the next statement

    e1->code(s, e);
    emit_push(T1, s);
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
    e2->code(s, e);
    emit_push(T2, s);
    emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
    emit_sub(T1, T1, T2, s);
    emit_blt(T1, ZERO, label_number, s);
    emit_load_address(ACC, BOOLCONST_PREFIX"0", s);  // false
    emit_jal_method(Object, copy_meth, s);
    emit_branch(label_number+1, s);  // the next statement

    emit_label_def(label_number++, s);  // true
    emit_load_address(ACC, BOOLCONST_PREFIX"0", s);
    emit_jal_method(Object, copy_meth, s);

    emit_label_def(label_number, s);
    emit_load(T1, 2, SP, s);
    emit_load(T2, 1, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void eq_class::code(ostream& s, env_type& e) {
    int label_number = e.blc;
    e.blc += 2;

    e1->code(s, e);
    emit_push(T1, s);
    emit_move(T1, ACC, s);
    e2->code(s, e);
    emit_push(T2, s);
    emit_move(T2, ACC, s);

    emit_load_address(ACC, BOOLCONST_PREFIX"1", s);
    emit_load_address(A1, BOOLCONST_PREFIX"0", s);
    emit_jal(EQUALITY_TEST, s);
}

void leq_class::code(ostream& s, env_type& e) {
    int label_number = e.blc;
    e.blc += 2;  // one for false, one for the next statement

    e1->code(s, e);
    emit_push(T1, s);
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
    e2->code(s, e);
    emit_push(T2, s);
    emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
    emit_sub(T1, T1, T2, s);
    emit_bleq(T1, ZERO, label_number, s);
    emit_load_address(ACC, BOOLCONST_PREFIX"0", s);  // false
    emit_jal_method(Object, copy_meth, s);
    emit_branch(label_number+1, s);  // the next statement

    emit_label_def(label_number++, s);  // true
    emit_load_address(ACC, BOOLCONST_PREFIX"1", s);
    emit_jal_method(Object, copy_meth, s);

    emit_label_def(label_number, s);
    emit_load(T1, 2, SP, s);
    emit_load(T2, 1, SP, s);
    emit_addiu(SP, SP, -8, s);
}

void comp_class::code(ostream& s, env_type& e) {
    e1->code(s, e);
    emit_push(T1, s);
    emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_neg(T1, T1, s);
    emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, -4, s);
}

void int_const_class::code(ostream& s, env_type& e)
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
}

void string_const_class::code(ostream& s, env_type& e)
{
  emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(ostream& s, env_type& e)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream& s, env_type& e) {
    Symbol tn = type_name;
    if (tn == SELF_TYPE) {
        tn = e.curr_class->get_name();
    }
    emit_partial_load_address(ACC, s);
    emit_protobj_ref(tn, s);
    s << endl;
    emit_jal_method(Object, copy_meth, s);
    s << JAL;
    emit_init_ref(tn, s);
    s << endl;
}

void isvoid_class::code(ostream& s, env_type& e) {
    int label_number = e.blc;
    e.blc += 2;

    e1->code(s, e);
    emit_bne(ACC, ZERO, label_number, s);
    emit_load_address(ACC, BOOLCONST_PREFIX"1", s);
    emit_jal_method(Object, copy_meth, s);
    emit_branch(label_number+1, s);

    emit_label_def(label_number++, s);
    emit_load_address(ACC, BOOLCONST_PREFIX"0", s);
    emit_jal_method(Object, copy_meth, s);
    emit_label_def(label_number, s);
}

void no_expr_class::code(ostream& s, env_type& e) {
    // this method does nothing, off course
}

void object_class::code(ostream& s, env_type& e) {
    env_type::id_type idtype;
    int offset;
    idtype = e.lookup_object(name, offset);
    switch (idtype) {
    case env_type::SELFOBJ:
        emit_move(ACC, SELF, s);
        break;
    case env_type::ATTRIBUTE:
        emit_store(ACC, offset, SELF, s);
        break;
    case env_type::FORMAL:
        emit_store(ACC, offset, FP, s);
        break;
    case env_type::LOCAL:
        emit_store(ACC, offset, FP, s);
        break;
    default:
        assert(0);
    }
}



// 
//

static void emit_class_names(ostream& s, CgenNodeP node) {
    s << WORD;
    stringtable.lookup_string(node->get_name()->get_string())->code_ref(s);
    s << endl;

    for (List<CgenNode>* c = node->get_children(); c; c = c->tl()) {
        emit_class_names(s, c->hd());
    }
}

static void emit_class_object_table(ostream& s, CgenNodeP node) {
    s << WORD;
    emit_protobj_ref(node->get_name(), s);
    s << endl;

    s << WORD;
    emit_init_ref(node->get_name(), s);
    s << endl;

    for (List<CgenNode>* c = node->get_children(); c; c = c->tl()) {
        emit_class_object_table(s, c->hd());
    }
}

/* To keep method order consistent, we iterate inherited methods reversely
 * and insert an inherited method to the front of method vector of the current
 * class if no overriding version is found. Otherwise, we replace it with
 * the overriding version.
 */
static void emit_dispatch_table(ostream& s, env_type& e, const std::vector<method_classname_pair>& ims) {
    s << e.curr_class->get_name() << DISPTAB_SUFFIX << LABEL;

    std::vector<method_classname_pair> pms;  // non-inherited methods

    Features features = e.curr_class->get_features();
    for (int i = features->first(); features->more(i); i = features->next(i)) {
        Feature ft = features->nth(i);
        if (ft->is_method()) {
            pms.push_back(std::make_pair(ft, e.curr_class->get_name()));
        }
    }
    for (std::vector<method_classname_pair>::const_reverse_iterator rit = ims.rbegin(); 
            rit != ims.rend(); ++rit) {
        std::vector<method_classname_pair>::iterator pos = 
            std::find_if(pms.begin(), pms.end(), method_name_is(rit->first->get_name()));
        if (pos != pms.end()) {   // overriding version
            pms.erase(pos);
            pms.insert(pms.begin(), std::make_pair(rit->first, e.curr_class->get_name()));
        } else {
            pms.insert(pms.begin(), *rit);
        }
    }
    int count = 0;
    for (std::vector<method_classname_pair>::iterator it = pms.begin(); it != pms.end(); ++it) {
        s << WORD;
        emit_method_ref(it->second, it->first->get_name(), s);
        s << endl;

#ifdef MYDEBUG
        std::cerr << "[moc] class: " << e.curr_class->get_name() << ", method: "
            << it->first->get_name() << ", offset: " << WORD_SIZE*count << std::endl;
#endif

        // construct method offset container
        e.set_method_offset(it->first->get_name(), count++);

        Feature curr_method_save = e.curr_method;
        e.curr_method = it->first;
        // avoid duplicate record
        if (it->second == e.curr_class->get_name()) {
            Formals formals = dynamic_cast<method_class*>(it->first)->get_formals();
            for (int j = formals->first(); formals->more(j); j = formals->next(j)) {
#ifdef MYDEBUG
                std::cerr << "[foc] class: " << e.curr_class->get_name() << ", method: "
                    << it->first->get_name() << ", formal: " << formals->nth(j)->get_name()
                    << ", offset: " << WORD_SIZE*(j+1) << std::endl;
#endif
                e.set_formal_offset(formals->nth(j)->get_name(), j+1);
            }
        }
        e.curr_method = curr_method_save;
    }
    for (List<CgenNode>* c = e.curr_class->get_children(); c; c = c->tl()) {
        CgenNodeP curr_class_save = e.curr_class;
        e.curr_class = c->hd();
        emit_dispatch_table(s, e, pms);
        e.curr_class = curr_class_save;
    }

}

static void emit_prototype_objects(ostream& s, env_type& e, const std::vector<Feature>& ias) {

    s << WORD << -1 << endl;    // eye catcher

    emit_protobj_ref(e.curr_class->get_name(), s);  // label
    s << LABEL;
    
    s << WORD << e.curr_class->get_class_tag() << endl;     // class tag

    Features features = e.curr_class->get_features();
    std::vector<Feature> pas(ias);
    for (int i = features->first(); features->more(i); i = features->next(i)) {
        if (!features->nth(i)->is_method()) {
            pas.push_back(features->nth(i));
        }
    }
    s << WORD << DEFAULT_OBJFIELDS + pas.size() << endl;  // object size

    s << WORD;
    emit_disptable_ref(e.curr_class->get_name(), s);    // dispatch table pointer
    s << endl;

    // attributs
    int offset = DEFAULT_OBJFIELDS;

    for (std::vector<Feature>::const_iterator it = pas.begin(); it != pas.end(); ++it) {
        s << WORD;
        attr_class* attr = dynamic_cast<attr_class*>(*it);
        if (attr->get_type() == Int) {
            inttable.lookup_string("0")->code_ref(s);
        } else if (attr->get_type() == Bool) {
            falsebool.code_ref(s);
        } else if (attr->get_type() == Str) {
            stringtable.lookup_string("")->code_ref(s);
        } else {
            s << 0; // void
        }
        s << endl;

#ifdef MYDEBUG
        std::cerr << "[aoc] class: " << e.curr_class->get_name() << ", attr: "
            << attr->get_name() << ", offset: " << WORD_SIZE*offset << std::endl;
#endif
        // construct offset container
        e.set_attr_offset(attr->get_name(), offset++);
    }

    for (List<CgenNode>* c = e.curr_class->get_children(); c; c = c->tl()) {
        CgenNodeP curr_class_save = e.curr_class;
        e.curr_class = c->hd();
        emit_prototype_objects(s, e, pas);
        e.curr_class = curr_class_save;
    }
}

static void gather_depth(env_type& e) {

    e.curr_class->gather_depth(e);

    for (List<CgenNode>* c = e.curr_class->get_children(); c; c = c->tl()) {
        CgenNodeP curr_class_save = e.curr_class;
        e.curr_class = c->hd();
        gather_depth(e);
        e.curr_class = curr_class_save;
    }
}

void class__class::gather_depth(env_type& e) {
    for (int i = features->first(); features->more(i); i = features->next(i)) {
        features->nth(i)->gather_depth(e);
    }
}

void attr_class::gather_depth(env_type& e) {
    int depth = init->gather_depth(e);
#ifdef MYDEBUG
    std::cerr << "[adc] class: " << e.curr_class->get_name() << ", attr: "
        << name << ", depth: " << depth << std::endl;
#endif
    e.set_attr_depth_info(depth);
}

void method_class::gather_depth(env_type& e) {
    e.curr_method = this;
    int depth = expr->gather_depth(e);
#ifdef MYDEBUG
    std::cerr << "[mdc] class: " << e.curr_class->get_name() << ", method: "
        << name << ", depth: " << depth << std::endl;
#endif
    e.set_method_depth_info(depth);
    e.curr_method = 0;
}

// case branches count
int branch_class::gather_depth(env_type& e) {
    return 1 + expr->gather_depth(e);
}

int assign_class::gather_depth(env_type& e) {
    return expr->gather_depth(e);
}

int static_dispatch_class::gather_depth(env_type& e) {
    int depth = expr->gather_depth(e);
    for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
        depth = std::max(depth, actual->nth(i)->gather_depth(e));
    }
    return depth;
}

int dispatch_class::gather_depth(env_type& e) {
    int depth = expr->gather_depth(e);
    for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
        depth = std::max(depth, actual->nth(i)->gather_depth(e));
    }
    return depth;
}

int cond_class::gather_depth(env_type& e) {
    return std::max(pred->gather_depth(e), std::max(then_exp->gather_depth(e),
            else_exp->gather_depth(e)));
}

int loop_class::gather_depth(env_type& e) {
    return std::max(pred->gather_depth(e), body->gather_depth(e));
}

int typcase_class::gather_depth(env_type& e) {
    int depth = -1;
    for (int i = cases->first(); cases->more(i); i = cases->next(i)) {
        depth = std::max(depth, cases->nth(i)->gather_depth(e));
    }
    return depth;
}

int block_class::gather_depth(env_type& e) {
    int depth = -1;
    for (int i = body->first(); body->more(i); i = body->next(i)) {
        depth = std::max(depth, body->nth(i)->gather_depth(e));
    }
    return depth;
}

// let statements count
int let_class::gather_depth(env_type& e) {
    return 1 + std::max(init->gather_depth(e), body->gather_depth(e));
}

int plus_class::gather_depth(env_type& e) {
    return std::max(e1->gather_depth(e), e2->gather_depth(e));
}

int sub_class::gather_depth(env_type& e) {
    return std::max(e1->gather_depth(e), e2->gather_depth(e));
}

int mul_class::gather_depth(env_type& e) {
    return std::max(e1->gather_depth(e), e2->gather_depth(e));
}

int divide_class::gather_depth(env_type& e) {
    return std::max(e1->gather_depth(e), e2->gather_depth(e));
}

int neg_class::gather_depth(env_type& e) {
    return e1->gather_depth(e);
}

int lt_class::gather_depth(env_type& e) {
    return std::max(e1->gather_depth(e), e2->gather_depth(e));
}

int eq_class::gather_depth(env_type& e) {
    return std::max(e1->gather_depth(e), e2->gather_depth(e));
}

int leq_class::gather_depth(env_type& e) {
    return std::max(e1->gather_depth(e), e2->gather_depth(e));
}

int comp_class::gather_depth(env_type& e) {
    return e1->gather_depth(e);
}

int int_const_class::gather_depth(env_type& e) {
    return 0;

}
int bool_const_class::gather_depth(env_type& e) {
    return 0;

}
int string_const_class::gather_depth(env_type& e) {
    return 0;
}

int new__class::gather_depth(env_type& e) {
    return 0;
}
int isvoid_class::gather_depth(env_type& e) {
    return e1->gather_depth(e);
}
int no_expr_class::gather_depth(env_type& e) {
    return 0;
}

int object_class::gather_depth(env_type& e) {
    return 0;
}

/*
 * Each initializer takes exactly one argument (self) which is passed
 * in $a0.
 *
 * illustrasion of a precedure's stack frame:
 *              | ...
 *              |                       <-- move $sp here before return
 *              | argument n
 *              | argument n-1
 *              | ...
 *              | argument 1
 *   new $fp -> | saved caller's $fp    <-- $sp just before enter this callee
 *              | saved $s0 ($a0/argument 0/self)
 *              | saved $ra
 *       $sp -> |
 *              | ...
 *
 *
 *  Where the new $fp should locate is not fixed and is totally up to the designer.
 */
static void emit_object_initializers(ostream& s, env_type& e) {
    emit_init_ref(e.curr_class->get_name(), s);
    s << LABEL;

    //
    // Things callers and callees should do to make consistency during
    // precudure calls.
    // Notice that the descriptions below is copied from SPIM's manual
    // thus may not be the same as our design!
    //
    // callee
    // 1. allocate memory for the frame
    // 2. save callee-saved registers in the frame ($s0-$s7, $fp, $ra).
    //    $ra only needs to be saved if the callee itself makes a call
    // 3. set $fp
    //    
    //    // function body of callee
    //
    // 4. save return value (into $a0 here)
    // 5. resotre callee-saved registers
    // 6. pop the stack frame
    // 7. jump to $ra
    //
    //
    // caller
    // 1. pass arguments ($a0-$a3 + stack)
    // 2. save caller-saved registers if needed ($a0-$a3, $t0-$t9)
    // 3. execute jal
    
    int depth = e.attr_depth_info();
    emit_precedure_set_up_code(s, depth);
    emit_move(SELF, ACC, s);

    CgenNodeP parent = e.curr_class->get_parentnd();
    if (parent && parent->get_name() != No_class) {
        s << JAL;
        emit_init_ref(parent->get_name(), s);
        s << endl;
    }

    Features features = e.curr_class->get_features();
    for (int i = features->first(); features->more(i); i = features->next(i)) {
        Feature ft = features->nth(i);
        if (ft->is_method()) {
            continue;
        }
        ft->code(s, e);
    }

    emit_move(ACC, SELF, s);
    emit_precedure_clean_up_code(s, depth);

    for (List<CgenNode>* c = e.curr_class->get_children(); c; c = c->tl()) {
        CgenNodeP curr_class_save = e.curr_class;
        e.curr_class = c->hd();
        emit_object_initializers(s, e);
        e.curr_class = curr_class_save;
    }
}

void attr_class::code(ostream& s, env_type& e) {
    if (init->get_type() == No_type) {
        return;
    }

    // create a new offset container for locals in the
    // initialization expression and set e.curr_loc points
    // to it
    env_type::local_offset_container loc;
    e.curr_loc = &loc;

    init->code(s, e);
    // save the address of the newly allocated object to the
    // corresponding location of the object
    if (init->get_type() != No_type) {
        emit_store(ACC, e.attr_offset(name), SELF, s);
    }

    // restore
    e.curr_loc = 0;
}

void method_class::code(ostream& s, env_type& e) {
    e.curr_method = this;  // important

    env_type::local_offset_container loc;
    e.curr_loc = &loc;

    int narg = formals->len();
    int depth = e.method_depth_info();
    emit_precedure_set_up_code(s, depth);
    emit_move(SELF, ACC, s);
    
    expr->code(s, e);

    emit_precedure_clean_up_code(s, depth, narg);

    e.curr_loc = 0;
    e.curr_method = 0;
}

static void emit_class_methods(ostream& s, env_type& e) {
    if (!e.curr_class->basic()) {   // skip built-in classes
        Features features = e.curr_class->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i)) {
            Feature ft = features->nth(i);
            if (!ft->is_method()) {
                continue;
            }
            emit_method_ref(e.curr_class->get_name(), ft->get_name(), s);
            s << LABEL;
            ft->code(s, e);
        }
    }

    for (List<CgenNode>* c = e.curr_class->get_children(); c; c = c->tl()) {
        CgenNodeP curr_class_save = e.curr_class;
        e.curr_class = c->hd();
        emit_class_methods(s, e);
        e.curr_class = curr_class_save;
    }
}


static void emit_precedure_set_up_code(ostream& s) {
    emit_precedure_set_up_code(s, 0);
}

static void emit_precedure_set_up_code(ostream& s, int depth) {
    emit_addiu(SP, SP, -WORD_SIZE*(FRAME_SIZE+depth), s);
    emit_store(FP, FRAME_SIZE+depth, SP, s);
    emit_store(SELF, FRAME_SIZE-1+depth, SP, s);
    emit_store(RA, FRAME_SIZE-2+depth, SP, s);
    emit_addiu(FP, SP, WORD_SIZE*(FRAME_SIZE+depth) ,s);
}

static void emit_precedure_clean_up_code(ostream& s) {
    emit_precedure_clean_up_code(s, 0, 0);
}

static void emit_precedure_clean_up_code(ostream& s, int depth) {
    emit_precedure_clean_up_code(s, depth, 0);
}

static void emit_precedure_clean_up_code(ostream& s, int depth, int narg) {
    emit_load(FP, FRAME_SIZE+depth, SP, s);
    emit_load(SELF, FRAME_SIZE-1+depth, SP, s);
    emit_load(RA, FRAME_SIZE-2+depth, SP, s);
    emit_addiu(SP, SP, WORD_SIZE*(FRAME_SIZE+narg), s);
    emit_return(s);
}
