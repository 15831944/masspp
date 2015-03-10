/// <summary>
/// [FILE] XmlReadWrite.cs
/// [ABSTRACT] LDIA for Pep plugin - XML file I/O class
/// Copyright (C) 2013-05-23 Shimadzu
/// </summary>

using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using System.Xml.Serialization;

//using LDIAforPepPlugin.ExecuteAnalysis.Data;

namespace LdiaTools.Other
{
    /// <summary> XML file I/O class </summary>
    public class XmlReadWrite
    {
        /// <summary>
        /// The output of the XML file.
        /// </summary>
        /// <typeparam name="T">Template class</typeparam>
        /// <param name="t">output class</param>
        public static void WriteXml<T>(ref T t)
        {
            string path = GetXmlFilePath(typeof(T));

            XmlSerializerNamespaces xmlSerializerNamespaces = new XmlSerializerNamespaces();
            xmlSerializerNamespaces.Add(String.Empty, String.Empty);

            XmlSerializer xmlSerializer = new XmlSerializer(typeof(T));
            Encoding encode = Encoding.GetEncoding("UTF-8");
            using (StreamWriter fs = new StreamWriter(path, false, encode))
            {
                xmlSerializer.Serialize(fs, t, xmlSerializerNamespaces);
            }
        }

        /// <summary>
        /// Reading of the XML file.
        /// </summary>
        /// <typeparam name="T">Template class</typeparam>
        /// <param name="t">output class</param>
        public static void ReadXml<T>(ref T t)
        {
            string path = GetXmlFilePath(typeof(T));

            XmlSerializer xmlSerializer = new XmlSerializer(typeof(T));

            try
            {
                using (FileStream fs = new FileStream(path, FileMode.Open))
                {
                    t = (T)xmlSerializer.Deserialize(fs);
                    fs.Close();
                }
            }
            catch (FileNotFoundException)
            {

            }
        }

        /// <summary>
        /// Get Setting XML File.
        /// </summary>
        /// <returns>XML File path</returns>
        private static string GetXmlFilePath(Type classType)
        {
            StringBuilder result = new StringBuilder();

            Assembly dllAssembly = Assembly.GetAssembly(classType);
            FileVersionInfo fvi = FileVersionInfo.GetVersionInfo(dllAssembly.Location);

            string companyName = fvi.CompanyName;
            string productName = fvi.ProductName;
            if ((Application.CompanyName != null) && (Application.ProductName != null))
            {
                if ((Application.CompanyName != string.Empty) && (Application.ProductName != string.Empty))
                {
                    companyName = Application.CompanyName;
                    productName = Application.ProductName;
                }
            }

            result.Append(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData));
            result.Append(@"\");
            result.Append(companyName);
            result.Append(@"\");
            result.Append(productName);
            result.Append(@"\");
            result.Append(Environment.UserName);

            Directory.CreateDirectory(result.ToString());

            result.Append(string.Format(@"\{0}.xml", classType.Name));
            return result.ToString();
        }
    }
}
