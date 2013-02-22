// Program.cs - create Excel add-in files
/*
If method returns reference to object, then insert this handle as first argument.!!!
*/
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Xml.Linq;
using System.Xml;

namespace xll
{
    class Program
    {
        static string copyright = "Copyright 2013 (c) KALX, LLC. No warranty is made. All rights reserved.";

        enum toc_type {XLL_NAME, C_NAME};
        static Dictionary<string, string[]> toc = new Dictionary<string, string[]>
        {
            {"Double", new string[] {"XLL_DOUBLEX", "double"}},
            {"Double[]", new string[] {"XLL_FPX", "_FP*"}},
            {"String", new string[] {"XLL_CSTRINGX", "xcstr"}},
            {"Boolean", new string[] {"XLL_BOOLX", "BOOL"}},
            {"UInt32", new string[] {"XLL_WORDX", "size_t"}},
            {"Object", new string[] {"XLL_HANDLEX", "HANDLEX"}},
        };

        // translate C# types C types
        static string CType(Type type)
        {
            string[] ctype;
            
            if (toc.TryGetValue(type.Name, out ctype))
                return ctype[(int)toc_type.C_NAME];

            return "HANDLEX";
        }
        // translate C# types to Excel types
        static string ExcelType(Type type)
        {
            string[] ctype;

            if (toc.TryGetValue(type.Name, out ctype))
                return ctype[(int)toc_type.XLL_NAME];

            return "XLL_HANDLEX";
        }

        // C# to C name
        static string c_name(string ac, string m)
        {
            return ac.ToLower().Replace('.', '_') + "_" + m.ToLower();
        }
        // C# to Excel name
        static string xll_name(string ac, string m)
        {
            return ac.ToUpper() + "." + m.ToUpper();
        }

        // write out class/category top level header and documentation file
        static string Header(string xll_dir, XElement members)
        {
            string h_pre = "// {0}.h - top level header\n" +
                "// " + copyright + "\n" +
                "// Uncomment the line below to compile for Excel 2007 and later.\n" +
                "//#define EXCEL12\n" +
                "#pragma once\n" +
                "#include \"xll/xll.h\"\n" +
                "\n#define CATEGORY _T(\"{0}\")\n" +
                "\ntypedef xll::traits<XLOPERX>::xcstr xcstr;\n";
            string c_pre = "// {0}.cpp - top level documentation\n" +
                "#include \"{0}.h\"\n" +
                "\nusing namespace xll;\n" +
                "\nstatic AddInX xai_{0}_document(\n"+
                "\tDocumentX(CATEGORY)\n"+
                "\t.Documentation(_T(\"{1}\"))\n"+
                ");\n";

            string category = null;

            foreach (var t in from m in members.Elements("member")
                              where m.Attribute("name").Value.StartsWith("T:")
                              select m)
            {
                string summary = t.Element("summary").Value;
                string name = t.Attribute("name").Value; // M:namespace.class

                name = name.Substring(2);
                string ns = name.Substring(0, name.IndexOf('.'));
                string cl = name.Substring(name.IndexOf('.') + 1);

                if (ns == cl)
                {
                    category = cl;
                    StreamWriter xll_h = new StreamWriter(String.Concat(xll_dir, category + ".h"));
                    xll_h.Write(h_pre, cl);
                    xll_h.Close();

                    StreamWriter xll_cpp = new StreamWriter(String.Concat(xll_dir, cl + ".cpp"));
                    xll_cpp.Write(c_pre, cl, summary.Trim().Replace("\n", "\")\n\t_T(\""));
                    xll_cpp.Close();

                    break;
                }
            }

            return category;
        }

        public class ParseType
        {
            public string namespace_;
            public string name_;
            public ParseType(Assembly xll_dll, string type)
            {
                Type t = xll_dll.GetType(type);
                namespace_ = t.Namespace;
                name_ = t.Name;
            }
            public string CPre(string name, string category)
            {
                string c_pre = "// {0}.cpp\n" +
                    "// " + copyright + "\n" +
                    "#include \"../{1}.h\"\n" +
                    "#include \"{2}.h\"\n" +
                    "\nusing namespace xll;\n";

                return String.Format(c_pre, name, name.ToLower(), category);
            }
        }

        public class ParseMethod
        {
            public string type_;
            public string method_;
            MethodInfo mi_;
            Type rt_; // return type
            bool static_; // true if static method
            public ParseMethod(Assembly xll_dll, string method)
            {
                if (method.Contains('('))
                    method = method.Substring(0, method.IndexOf('('));

                method_ = method.Substring(method.LastIndexOf('.') + 1);
                type_ = method.Substring(0, method.LastIndexOf('.'));
                Type t = xll_dll.GetType(type_);
                mi_ = t.GetMethod(method_);
                if (mi_ != null && toc.ContainsKey(mi_.ReturnType.Name))
                {
                    rt_ = mi_.ReturnType;
                    static_ = mi_.IsStatic;
                }
                else
                {
                    rt_ = typeof(System.Object); // handle
                }
            }

            // C declaration: return_type name(arg_type arg,...)
            public string Decl()
            {
                string c_decl = "{1} WINAPI xll_{0}({2})";
                string args = static_ ? "" : "HANDLEX h, ";

                if (mi_ != null)
                {
                    ParameterInfo[] pi = mi_.GetParameters();
                    for (int i = 0; i < pi.Length; ++i)
                    {
                        if (i > 0)
                        {
                            args += ", ";
                        }
                        args += CType(pi[i].ParameterType);
                        args += " ";
                        args += pi[i].Name;
                    }
                }

                return String.Format(c_decl, c_name(type_, method_), CType(rt_), args);
            }
            // AddIn definition
            public string AddIn(XElement member)
            {
                string addin_decl = "static AddInX xai_{0}(\n" +
                    "\tFunctionX({1}, _T(\"?xll_{0}\"), _T(\"{2}\"))\n" +
                    "{3}" +
                    "\t.Category(CATEGORY)\n" +
                    "\t.FunctionHelp(_T(\"{4}\"))\n" +
                    "\t.Documentation(_T(\"{5}\"))\n" +
                    ");";

                //!!! add name of underlying class
                string Args = static_ ? "" : "\t.Arg(XLL_HANDLEX, _T(\"Handle\"), _T(\"is a handle\"))\n";
                if (mi_ != null)
                {
                    ParameterInfo[] pi = mi_.GetParameters();
                    var param = member.Elements("param").ToArray();
                    for (int i = 0; i < param.Length; ++i)
                    {
                        Args += "\t.Arg(" + ExcelType(pi[i].ParameterType);
                        Args += ", _T(\"" + param[i].Attribute("name").Value + "\")";
                        Args += ", _T(\"" + param[i].Value + "\"))\n";
                    }
                }

                return String.Format(addin_decl, c_name(type_, method_),
                    ExcelType(rt_),
                    xll_name(type_, method_), Args, 
                    member.Element("summary").Value.Trim(), 
                    member.Element("remarks").Value.Trim().Replace("\n", "\")\n\t_T(\""));
            }
            public string Body(XElement member)
            {
                string c_impl = "{0}\n"+
                    "{{\n" +
                    "#pragma XLLEXPORT\n" +
                    "\t{1} result(0);\n\n" +
                    "\ttry {{\n" +
                    "\t\t{2}\n" +
                    "\t}}\n" +
                    "\tcatch (const std::exception& ex) {{\n" +
                    "\t\tXLL_ERROR(ex.what());\n" +
                    "\t}}\n\n" +
                    "\treturn result;\n" +
                    "}}\n";

                string body = null;

                XElement Body = member.Element("body");
                if (Body != null)
                    body = Body.Value;

                if (body == null)
                {
                    XElement Result = member.Element("result");
                    if (Result != null)
                        body = "result = " + Result.Value + ";";
                }

                if (body == null) // make a stab
                {
                }

                if (method_ == "#ctor")
                {
                    body = String.Format(c_impl, Decl(), CType(rt_), body);
                }
                else
                {
                    body = String.Format(c_impl, Decl(), CType(rt_), body);
                }

                return body;
            }

        }

        static void Main(string[] args)
        {
            foreach (string file in args)
            {
                string xll_dir = "../" + file + "/" + file + "_xll/";
                XDocument xll_xml = XDocument.Load(String.Concat(file, @".xml"), LoadOptions.None);
                Assembly xll_dll = Assembly.LoadFrom(String.Concat(file, @".dll"));

                string assembly = xll_xml.Element("doc").Element("assembly").Value;
                XElement members = xll_xml.Element("doc").Element("members");

                // create top level header
                string category = Header(xll_dir, members);

                foreach (var t in from m in members.Elements("member")
                            where m.Attribute("name").Value.StartsWith("T:")
                            select m)
                {
                    string name = t.Attribute("name").Value; // M:namespace.class
                    string summary = t.Element("summary").Value;
                    name = name.Substring(2);
                    ParseType pt = new ParseType(xll_dll, name);

                    if (pt.name_ == category)
                        continue;

                    name = name.Substring(category.Length + 1);
                    StreamWriter xll_c = new StreamWriter(String.Concat(xll_dir, name.Replace(".","_"), @".cpp"));
                    xll_c.WriteLine(pt.CPre(name, category));

                    foreach (var m in from m in members.Elements("member")
                                        where m.Attribute("name").Value.StartsWith("M:" + pt.namespace_ + "." + pt.name_)
                                        select m)
                    {
                        string macm = m.Attribute("name").Value; // M:Assembly.Class.Method(Args)
                        macm = macm.Substring(2);

                        ParseMethod pm = new ParseMethod(xll_dll, macm);
                        xll_c.WriteLine(pm.Body(m));
                    }

                    xll_c.Close();
                } 
            }
        }
    }
}
