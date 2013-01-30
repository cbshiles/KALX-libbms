// Program.cs - create Excel add-in files
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
    // C# to Excel and C types
    public enum toc_type { XLL_NAME, C_NAME };

    class Program
    {
        // translate C# types to Excel and C types
        static Dictionary<Type, string[]> toc = new Dictionary<Type, string[]> {
            {typeof(System.Boolean), new string[] {"XLL_BOOLX", "BOOL"}},
            {typeof(System.Double), new string[] {"XLL_DOUBLEX", "double"}},
            {typeof(System.Double[]), new string[] {"XLL_FPX", "_FP*"}},
            {typeof(System.UInt32), new string[] {"XLL_WORDX", "size_t"}}
        };

        static string copyright = "Copyright 2013 (c) KALX, LLC. No warranty is made. All rights reserved.";
        static string h_pre = "// {0}.h - top level header\n" +
            "// " + copyright + "\n" +
            "// Uncomment the line below to compile for Excel 2007 and later.\n" +
            "//#define EXCEL12\n" +
            "#pragma once\n" +
            "#include \"xll/xll.h\"\n"+
            "\n#define CATEGORY _T(\"{0}\")\n";
        static string c_pre = "// {0}.cpp\n" +
            "// " + copyright + "\n" +
            "#include \"../{1}.h\"\n" +
            "#include \"{2}\"\n" +
            "\nusing namespace xll;\n";
        static string c_decl = "{1} WINAPI xll_{0}({2})";
        static string addin_decl = "static AddInX xai_{0}(\n" +
            "\tFunctionX({1}, _T(\"?xll_{0}\"), _T(\"{2}\"))\n" +
            "{3}" +
            "\t.Category(_T(\"{4}\"))\n" +
            "\t.FunctionHelp(_T(\"{5}\"))\n" +
            "\t.Documentation(_T(\"{6}\"))\n" +
            ");";
        static string c_impl = "{0}\n"+
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
        static string top_level = "// {0}.cpp - top level documentation\n" +
            "#include \"{0}.h\"\n" +
            "\nusing namespace xll;\n" +
            "\nstatic AddInX xai_{0}_document(\n"+
            "\tDocumentX(CATEGORY)\n"+
            "\t.Documentation(_T(\"{1}\"))\n"+
            ");\n";

        static string c_name(string ac, string m)
        {
            return ac.ToLower().Replace('.', '_') + "_" + m.ToLower();
        }
        static string xll_name(string ac, string m)
        {
            return ac.ToUpper() + "." + m.ToUpper();
        }

        static void Main(string[] args)
        {
            foreach (string file in args)
            {
                string xll_dir = "../" + file + "/" + file + "_xll/";
                Assembly xll_dll = Assembly.LoadFrom(String.Concat(file, @".dll"));
                XDocument xll_xml = XDocument.Load(String.Concat(file, @".xml"), LoadOptions.None);

                string assembly = xll_xml.Element("doc").Element("assembly").Value;
                XElement members = xll_xml.Element("doc").Element("members");

                string top_header = null;
                // create top level header
                foreach (var t in from m in members.Elements("member")
                                  where m.Attribute("name").Value.StartsWith("T:")
                                  select m)
                {
                    string name = t.Attribute("name").Value; // M:namespace.class
                    string summary = t.Element("summary").Value;
                    name = name.Substring(2);
                    string ns = name.Substring(0, name.IndexOf('.'));
                    string cl = name.Substring(name.IndexOf('.') + 1);

                    if (ns == cl)
                    {
                        top_header = cl + ".h";
                        StreamWriter xll_h = new StreamWriter(String.Concat(xll_dir, top_header));
                        xll_h.Write(h_pre, cl);
                        xll_h.Close();

                        StreamWriter xll_cpp = new StreamWriter(String.Concat(xll_dir, cl + ".cpp"));
                        xll_cpp.Write(top_level, cl, summary.Trim().Replace("\n", "\")\n\t_T(\""));
                        xll_cpp.Close();

                        break;
                    }
                }

                Debug.Assert(top_header != null);

                foreach (var t in from m in members.Elements("member")
                            where m.Attribute("name").Value.StartsWith("T:")
                            select m)
                {
                    string name = t.Attribute("name").Value; // M:namespace.class
                    string summary = t.Element("summary").Value;
                    name = name.Substring(2);
                    string ns = name.Substring(0, name.IndexOf('.'));
                    string cl = name.Substring(name.IndexOf('.') + 1);

                    if (ns == cl)
                    {
                        continue; // skip top level
                    }

                    StreamWriter xll_c = new StreamWriter(String.Concat(xll_dir, cl, @".cpp"));

                    xll_c.WriteLine(c_pre, cl, cl.ToLower(), top_header);

                    foreach (var m in from m in members.Elements("member")
                                      where m.Attribute("name").Value.StartsWith("M:" + name)
                                      select m)
                    {
                        string macm = m.Attribute("name").Value; // M:Assembly.Class.Method(Args)
                        string acm = macm.Substring(2, macm.IndexOf('(') - 2); // Assembly.Class.Method
                        string ac = acm.Substring(0, acm.LastIndexOf('.')); // Assembly.Class
                        string category = ac.Substring(0, ac.LastIndexOf('.')); // Assembly
                        string method = acm.Substring(acm.LastIndexOf('.') + 1); // Method

                        MethodInfo mi = xll_dll.GetType(ac).GetMethod(method);
                        string decl = Decl(mi, ac, method);

                        Type rt = mi.ReturnType.UnderlyingSystemType;
                        var param = m.Elements("param").ToArray();
                        ParameterInfo[] pi = mi.GetParameters();

                        Debug.Assert(param.Length == pi.Length);

                        string Args = "";
                        for (int i = 0; i < param.Length; ++i)
                        {
                            Args += "\t.Arg(" + toc[pi[i].ParameterType][(int)toc_type.XLL_NAME];
                            Args += ", _T(\"" + param[i].Attribute("name").Value + "\")";
                            Args += ", _T(\"" + param[i].Value + "\"))\n";
                        }
                        xll_c.WriteLine(addin_decl, c_name(ac, method), 
                            toc[rt][(int)toc_type.XLL_NAME], xll_name(ac, method),
                            Args, category, m.Element("summary").Value.Trim(), m.Element("remarks").Value.Trim().Replace("\n", "\")\n\t_T(\""));

                        string body = "<body>";
                        
                        XElement Body = m.Element("body");
                        if (Body != null)
                            body = Body.Value;

                        if (Body == null)
                        {
                            XElement Result = m.Element("result");
                            if (Result != null)
                                body = "result = " + Result.Value + ";";
                        }
                        
                        xll_c.WriteLine(c_impl, decl, toc[rt][(int)toc_type.C_NAME], body);
                    }

                    xll_c.Close();
                }
            }
        }

        // return_type name(arg_type arg,...)
        static string Decl(MethodInfo mi, string ac, string method)
        {
            string args = "";
            ParameterInfo[] pi = mi.GetParameters();
            for (int i = 0; i < pi.Length; ++i)
            {
                if (i > 0)
                {
                    args += ", ";
                }
                args += toc[pi[i].ParameterType][(int)toc_type.C_NAME];
                args += " ";
                args += pi[i].Name;
            }

            string result = toc[mi.ReturnType.UnderlyingSystemType][(int)toc_type.C_NAME];

            return String.Format(c_decl, c_name(ac, method), result, args);
        }

        // name(arg,...)
        static string Call(MethodInfo mi, string ac, string method)
        {
            string call = c_name(ac, method) + "(";

            ParameterInfo[] pi = mi.GetParameters();
            for (int i = 0; i < pi.Length; ++i)
            {
                if (i > 0)
                {
                    call += ", ";
                }

                // FP data type
                if (pi[i].ParameterType.IsArray && pi[i].ParameterType.UnderlyingSystemType == typeof(System.Double))
                {
                    call += "size(*" + pi[i].Name + "), " + pi[i].Name + "->array";
                }
                else
                {
                    call += pi[i].Name;
                }
            }

            call += ")";

            return call;
        }
    }
}
