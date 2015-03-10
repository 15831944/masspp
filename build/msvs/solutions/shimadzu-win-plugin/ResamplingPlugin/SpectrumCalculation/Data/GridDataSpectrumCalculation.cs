/// <summary>
/// [FILE] GridDataSpectrumCalculation.cs
/// [ABSTRACT] Resampling plugin - Data class for Spectrum Calculation DataGridView
/// Copyright (C) 2013-08-23 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ResamplingPlugin.ResamplingTools.Data;
using kome.clr;
using System.Diagnostics;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Data;
using System.IO;
using System.ComponentModel;

namespace ResamplingPlugin.SpectrumCalculation.Data
{
    /// <summary>GridDataSpectrumCalculation class</summary>
    public class GridDataSpectrumCalculation
    {
        #region --- Structure ------------------------------------------
        /// <summary>SelSpectrum structure</summary>
        private struct SelSpectrum
        {
            /// <summary>Sample Tab Number(If there are some samples in selected file,the munber of sample that includes selected spectrum)</summary>
            internal int SampleTabNo;
            /// <summary>Folder(The path of file that includes selected spectrum)</summary>
            internal string Folder;
            /// <summary>File Name(The name of file that includes selected spectrum)</summary>
            internal string FileName;
            /// <summary>Spectrum Name(The name of selected spectrum)</summary>
            internal string SpecName;
            /// <summary>RT(The RT value of selected spectrum)</summary>
            internal string RT;
            /// <summary>Precusor(The Precusor value of parents of selected spectrum)</summary>
            internal string Precusor;
            /// <summary>TotalSpecNum(The total spectrum number in file that includes selected spectrum)</summary>
            internal uint TotalSpecNum;
            /// <summary>Descendant List(The history of descendant(node) of selected spectrum,
            /// e.g. selected spectrum is third child of father(or mother) that is second child of grandfather(or grandmother))</summary>
            internal List<uint> DescendantList;
            /// <summary>Descendant (selected spectrum is, not child(0),or child(1),or grandchild(2).....and so on) </summary>
            internal uint Descendant;
            /// <summary>Child number(first,or second,or third....child) </summary>
            internal uint ChildNum;
            /// <summary>Spectrum number(first,or second,or third....spectrum of that node) </summary>
            internal uint SpecNum;

            /// <summary>
            /// Initialize SelSpectrum structure.
            /// </summary>
            /// <param name="sampleTabNo">Sample Tab Number</param>
            /// <param name="folder">Folder</param>
            /// <param name="fileName">File Name</param>
            /// <param name="specName">Spectrum Name</param>
            /// <param name="rt">RT</param>
            /// <param name="precusor">Precusor</param>
            /// <param name="totalSpecNum">TotalSpecNum</param>
            /// <param name="descendantList">Descendant List</param>
            /// <param name="descendant">Descendant</param>
            /// <param name="childeNum">Child number</param>
            /// <param name="specNum">Spectrum number</param>
            internal SelSpectrum(int sampleTabNo, string folder, string fileName, string specName, string rt,
                string precusor, uint totalSpecNum, List<uint> descendantList, uint descendant, uint childeNum, uint specNum)
            {
                this.SampleTabNo = sampleTabNo;
                this.Folder = folder;
                this.FileName = fileName;
                this.SpecName = specName;
                this.RT = rt;
                this.Precusor = precusor;
                this.TotalSpecNum = totalSpecNum;
                this.DescendantList = descendantList;
                this.Descendant = descendant;
                this.ChildNum = childeNum;
                this.SpecNum = specNum;
            }
        }

        /// <summary>ReadSpectrumData structure</summary>
        private struct ReadSpectrumData
        {
            /// <summary>Spectrum is read from file </summary>
            internal SpectrumWrapper Spec;
            /// <summary>Descendant List(The history of descendant(node) of spectrum is read from file,
            /// e.g. selected spectrum is third child of father(or mother) that is second child of grandfather(or grandmother))</summary>
            internal List<uint> DescendantList;
            /// <summary>Descendant (spectrum is read from file is, not child(0),or child(1),or grandchild(2).....and so on) </summary>
            internal uint Descendant;
            /// <summary>Child number(first,or second,or third....child) </summary>
            internal uint ChildNum;
            /// <summary>Spectrum number(first,or second,or third....spectrum of that node) </summary>
            internal uint SpecNum;

            /// <summary>
            /// Initialize ReadSpectrumData structure.
            /// </summary>
            /// <param name="spec">Spectrum is read from file</param>
            /// <param name="descendantList">Descendant List</param>
            /// <param name="descendant">Descendant</param>
            /// <param name="childNum">Child number</param>
            /// <param name="specNum">Spectrum number</param>
            internal ReadSpectrumData(SpectrumWrapper spec, List<uint> descendantList, uint descendant, uint childNum, uint specNum)
            {
                Spec = spec;
                DescendantList = descendantList;
                Descendant = descendant;
                ChildNum = childNum;
                SpecNum = specNum;
            }
        }

        /// <summary>FileManage structure(memory of opend file to close after)</summary>
        private struct FileManage
        {
            internal string FileName;
            internal FileFactoryWrapper FileFactory;
            internal SampleSetWrapper SampleSet;
        }

        /// <summary>FileData structure</summary>
        private struct FileData
        {
            /// <summary>Sample Tab Number(If there are some samples in selected file)</summary>
            internal int SampleTabNo;
            /// <summary>Folder(The path of selected file)</summary>
            internal string Folder;
            /// <summary>File Name(The name of selected file)</summary>
            internal string FileName;
            /// <summary>SpectrumNameData List(The infomation lists of Spectrums in selected file)</summary>
            internal List<SpectrumData> SpectrumNameDataList;
            /// <summary>Spectrum Name List for combobox items</summary>
            internal BindingList<string> SpectrumNameList;

            /// <summary>
            /// Initialize FileData Structure.
            /// </summary>
            /// <param name="sampleTabNo"></param>
            /// <param name="folder"></param>
            /// <param name="fileName"></param>
            /// <param name="spectrumNameDataList"></param>
            /// <param name="spectrumNameList"></param>
            internal FileData(int sampleTabNo, string folder, string fileName)
            {
                SampleTabNo = sampleTabNo;
                Folder = folder;
                FileName = fileName;
                SpectrumNameDataList = new List<SpectrumData>();
                SpectrumNameList = new BindingList<string>();
            }
        }

        /// <summary>SpectrumData structure</summary>
        private struct SpectrumData
        {
            /// <summary>
            /// Spectrum name.
            /// </summary>
            internal string SpectrumName;

            /// <summary>
            /// RT value.
            /// </summary>
            internal double RT;

            /// <summary>
            /// Precursor string.
            /// </summary>
            internal string Precusor;
        }

        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>SelSpectrum List</summary>
        private List<SelSpectrum> _selSpectrum = new List<SelSpectrum>();

        /// <summary>FileMange List</summary>
        private List<FileManage> _listFileManage = new List<FileManage>();

        /// <summary>File Name BindingSource for combobox</summary>
        private BindingSource _bindFileName;

        /// <summary>File Name BindingSource for combobox of last row</summary>
        private BindingSource _bindFileNameLastRow;

        /// <summary>FileNameData List</summary>
        List<FileData> _fileNameDataList;

        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the GridDataSpectrumCalculation class
        /// </summary>
        public GridDataSpectrumCalculation()
        {

            _fileNameDataList = new List<FileData>();

            _bindFileName = new BindingSource();
            _bindFileNameLastRow = new BindingSource();
            _bindFileNameLastRow.Add("(Select MS File)");
            _bindFileNameLastRow.Add("Browse...");
        }
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary>get file name BindingSource for combobox</summary>
        public BindingSource GetFileNameList
        {
            get { return _bindFileName; }
        }

        /// <summary>get file name BindingSource for combobox of last row</summary>
        public BindingSource GetFileNameListBottom
        {
            get { return _bindFileNameLastRow; }
        }

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Close opend files (when file is closed,stocked spectrums are not used,so files are still opened during calculation)
        /// </summary>
        public void CloseFileManageList()
        {
            foreach (FileManage fm in _listFileManage)
            {
                if (fm.SampleSet.isOpened() == true)
                {
                    fm.FileFactory.closeFile(fm.SampleSet);
                }
            }
            _listFileManage.Clear();
        }

        /// <summary>
        /// Get Spectrum Data(selected by File and Spectrum Name and RT value and Precusor value)
        /// </summary>
        /// <param name="nameDataListIndex">index number of file name combobox</param>
        /// <param name="specName">Spectrum Name(selected combobox or determinded by selected RT and Precusor combobox) </param>
        /// <param name="rt">RT value (selected combobox or determinded by selected Spectrum Name combobox) </param>
        /// <param name="precusor">Precusor value (selected combobox or determinded by selected Spectrum Name combobox) </param>
        /// <returns>SpectrumWrapper</returns>
        public SpectrumWrapper GetSpectrumWrapperData(int nameDataListIndex, string specName, string rt, string precusor)
        {
            string folder = _fileNameDataList[nameDataListIndex].Folder;
            string fileName = _fileNameDataList[nameDataListIndex].FileName;
            int sampleNum = _fileNameDataList[nameDataListIndex].SampleTabNo;

            var pred = new Predicate<SelSpectrum>(x =>
                (x.Folder == folder
                && x.FileName == fileName
                && x.SpecName == specName
                && x.RT == rt
                && x.Precusor == precusor
                && x.SampleTabNo == sampleNum));
            int find = _selSpectrum.FindIndex(pred);

            // error
            if (find < 0)
            {
                return null;
            }

            string tfp = Path.Combine(folder, fileName);
            FileFactoryWrapper factory = FileFactoryWrapper.getInstance();
            SampleSetWrapper set = factory.openFile(tfp);
            SampleWrapper sample = set.getSample((uint)_selSpectrum[find].SampleTabNo - 1);
            sample.openSample();
            DataGroupNodeWrapper targetNode = sample.getRootDataGroupNode();

            int listcount = _selSpectrum[find].DescendantList.Count;

            for (int cnt = 0; cnt < listcount; cnt++)
            {
                targetNode = targetNode.getChild(_selSpectrum[find].DescendantList[cnt]);
            }

            SpectrumWrapper spec = targetNode.getSpectrum(_selSpectrum[find].SpecNum);

            var predfn = new Predicate<FileManage>(x => (x.FileName == tfp));
            find = _listFileManage.FindIndex(predfn);

            if (find < 0)
            {
                FileManage fmanage;
                fmanage.FileFactory = factory;
                fmanage.SampleSet = set;
                fmanage.FileName = tfp;
                _listFileManage.Add(fmanage);
            }
            // Not closed File,because stocked spectrum data read from file not used when file is closed
            return spec;
        }

        /// <summary>
        /// Get Spectrum Name List for combobox items
        /// </summary>
        /// <param name="index">index of File Name Combobox</param>
        /// <returns>BindingList of string(Spectrum Name) </returns>
        public BindingList<string> GetSpectrumNameList(int index)
        {
            return _fileNameDataList[index].SpectrumNameList;
        }

        /// <summary>
        /// Get Spectrum Name for textbox(determinded by selected RT and Precusor combobox)
        /// </summary>
        /// <param name="fileNameIndex">index of File Name Combobox</param>
        /// <param name="rt">RT value</param>
        /// <param name="precursor">Precursor value</param>
        /// <returns>string (Spectrum name)</returns>
        public string GetSpecturmName_TextBox(int fileNameIndex, string rt, string precursor)
        {
            string specturmName = "";
            string folder = _fileNameDataList[fileNameIndex].Folder;
            string fileName = _fileNameDataList[fileNameIndex].FileName;
            int smplNum = _fileNameDataList[fileNameIndex].SampleTabNo;
            string strPrecursorOrg = "";
            string strPrecursorNum = "";
            int startn = precursor.IndexOf("(");
            int endn = precursor.IndexOf(")");
            int precursorNum = -1;
            // same precursor value(e.g. "-----") is renamed "name" + "(number)",so remove "(number)" for original name
            // number is memorized separately
            if ((startn != -1) && (endn != -1))
            {
                strPrecursorOrg = precursor.Substring(0, startn);
                strPrecursorNum = precursor.Substring(startn + 1, endn - startn - 1);
                precursorNum = int.Parse(strPrecursorNum);
            }
            else
            {
                strPrecursorOrg = precursor;
            }

            var pred = new Predicate<SelSpectrum>(x =>
                (x.Folder == folder
                && x.FileName == fileName
                && x.RT == rt
                && x.Precusor == strPrecursorOrg
                && x.SampleTabNo == smplNum));

            int find = 0;
            int num = 0;
            for (int n = 0; n < _selSpectrum.Count && find < _selSpectrum.Count; n++, find++)
            {
                find = _selSpectrum.FindIndex(find, pred);
                if (find >= 0)
                {
                    specturmName = _selSpectrum[find].SpecName;
                    num++;
                    if (precursorNum == num)
                        break;
                }
                else
                {
                    break;
                }
            }
            return specturmName;

        }

        /// <summary>
        /// Get RT value for textbox(determinded by selected Spectrum Name combobox)
        /// </summary>
        /// <param name="nameDataListIndex">index of File Name Combobox</param>
        /// <param name="spectrumNameDataListIndex">index of Spectrum Name Combobox</param>
        /// <returns>double (RT value)</returns>
        public double GetRT_TextBoxData(int nameDataListIndex, int spectrumNameDataListIndex)
        {
            return _fileNameDataList[nameDataListIndex].SpectrumNameDataList[spectrumNameDataListIndex].RT;
        }

        /// <summary>
        /// Get Precusor value for textbox(determinded by selected Spectrum Name combobox)
        /// </summary>
        /// <param name="nameDataListIndex">index of File Name Combobox</param>
        /// <param name="spectrumNameDataListIndex">index of Spectrum Name Combobox</param>
        /// <returns>string (Precusor value + "(number)")</returns>
        public string GetPrecusor_TextBox(int nameDataListIndex, int spectrumNameDataListIndex)
        {
            return _fileNameDataList[nameDataListIndex].SpectrumNameDataList[spectrumNameDataListIndex].Precusor;
        }

        /// <summary>
        /// Get RT List for combobox items
        /// </summary>
        /// <param name="index">index of File Name Combobox</param>
        /// <returns>List of string(RT value)</returns>
        public List<string> GetRT_Combobox(int index)
        {
            List<string> rtList_ComboBox = new List<string>();

            string folder = _fileNameDataList[index].Folder;
            string fileName = _fileNameDataList[index].FileName;
            int smplNum = _fileNameDataList[index].SampleTabNo;

            var pred = new Predicate<SelSpectrum>(x =>
                (x.Folder == folder
                && x.FileName == fileName
                && x.SampleTabNo == smplNum));

            int find = 0;
            for (int n = 0; n < _selSpectrum.Count && find < _selSpectrum.Count; n++, find++)
            {
                find = _selSpectrum.FindIndex(find, pred);

                if (find >= 0)
                {
                    string RT = _selSpectrum[find].RT;
                    var predRT = new Predicate<String>(x => (x == RT));
                    if (rtList_ComboBox.FindIndex(predRT) < 0)
                    {
                        rtList_ComboBox.Add(RT);
                    }

                }
                else
                {
                    break;
                }
            }
            return rtList_ComboBox;
        }

        /// <summary>
        /// Get RT combobox item vaule (determinded by selected spectrum name)
        /// </summary>
        /// <param name="index">index of File Name Combobox</param>
        /// <param name="specName">spectrum name</param>
        /// <returns>string (RT value)</returns>
        public string GetRT_Combobox(int index, string specName)
        {
            string folder = _fileNameDataList[index].Folder;
            string fileName = _fileNameDataList[index].FileName;
            int smplNum = _fileNameDataList[index].SampleTabNo;

            var predSpec = new Predicate<SelSpectrum>(x =>
                (x.Folder == folder
                && x.FileName == fileName
                && x.SampleTabNo == smplNum
                && x.SpecName == specName));

            int findSpec = _selSpectrum.FindIndex(predSpec);
            return _selSpectrum[findSpec].RT;
        }

        /// <summary>
        /// Get Precursor List for combobox items
        /// </summary>
        /// <param name="fileNameIndex">index of File Name Combobox</param>
        /// <param name="rt">RT value</param>
        /// <returns>List of string(Precursor value)</returns>
        public List<string> GetPrecursor_ComboBox(int fileNameIndex, string rt)
        {
            List<string> precursorList = new List<string>();
            string folder = _fileNameDataList[fileNameIndex].Folder;
            string fileName = _fileNameDataList[fileNameIndex].FileName;
            int sampleNumber = _fileNameDataList[fileNameIndex].SampleTabNo;

            var pred = new Predicate<SelSpectrum>(x =>
                (x.Folder == folder
                && x.FileName == fileName
                && x.RT == rt
                && x.SampleTabNo == sampleNumber));

            int find = 0;
            for (int n = 0; n < _selSpectrum.Count && find < _selSpectrum.Count; n++, find++)
            {
                find = _selSpectrum.FindIndex(find, pred);

                if (find >= 0)
                {
                    string Precusor = _selSpectrum[find].Precusor;
                    precursorList.Add(Precusor);
                }
                else
                {
                    break;
                }
            }
            bool[] chk = new bool[precursorList.Count];
            chk[0] = false;
            for (int n = 0; n < precursorList.Count - 1; n++)
            {
                string org;
                int num = 1;
                if (chk[n] != true)
                {
                    org = precursorList[n];
                    for (int m = n + 1; m < precursorList.Count; m++)
                    {
                        if (org == precursorList[m])
                        {
                            num++;
                            precursorList[m] = org + "(" + num.ToString() + ")";
                        }
                        chk[m] = true;
                    }
                    if (num != 1)
                    {
                        precursorList[n] = org + "(1)";
                    }
                }
            }
            return precursorList;
        }

        /// <summary>
        /// Get Precursor List for combobox items and selected index(determinded by selected spectrum name)
        /// </summary>
        /// <param name="fileNameIndex">index of File Name Combobox</param>
        /// <param name="rt">RT value</param>
        /// <param name="specName">spectrum name</param>
        /// <param name="selIndex">selected index that determinded by selected spectrum name</param>
        /// <returns>List of string(Precursor value)</returns>
        public List<string> GetPrecursor_ComboBox(int fileNameIndex, string rt, string specName, ref int selIndex)
        {
            List<string> precursorList = new List<string>();
            string folder = _fileNameDataList[fileNameIndex].Folder;
            string fileName = _fileNameDataList[fileNameIndex].FileName;
            int idx = 0;
            int smplNum = _fileNameDataList[fileNameIndex].SampleTabNo;

            var pred = new Predicate<SelSpectrum>(x =>
                (x.Folder == folder
                && x.FileName == fileName
                && x.RT == rt
                && x.SampleTabNo == smplNum));
            var predSpec = new Predicate<SelSpectrum>(x =>
                (x.Folder == folder
                && x.FileName == fileName
                && x.RT == rt
                && x.SampleTabNo == smplNum
                && x.SpecName == specName));
            int findSpec = _selSpectrum.FindIndex(predSpec);

            int find = 0;
            for (int n = 0; n < _selSpectrum.Count && find < _selSpectrum.Count; n++, find++)
            {
                find = _selSpectrum.FindIndex(find, pred);

                if (find >= 0)
                {
                    string Precusor = _selSpectrum[find].Precusor;
                    precursorList.Add(Precusor);
                    if (find == findSpec)
                    {
                        selIndex = idx;
                    }
                    idx++;
                }
                else
                {
                    break;
                }
            }

            bool[] chk = new bool[precursorList.Count];
            chk[0] = false;
            for (int n = 0; n < precursorList.Count - 1; n++)
            {
                string org;
                int num = 1;
                if (chk[n] != true)
                {
                    org = precursorList[n];
                    for (int m = n + 1; m < precursorList.Count; m++)
                    {
                        if (org == precursorList[m])
                        {
                            num++;
                            precursorList[m] = org + "(" + num.ToString() + ")";
                        }
                        chk[m] = true;
                    }
                    if (num != 1)
                    {
                        precursorList[n] = org + "(1)";
                    }
                }
            }
            return precursorList;
        }


        /// <summary>
        /// Read MSB Data File
        /// </summary>
        /// <param name="filepath">MSB file Path</param>
        /// <returns></returns>
        public int ReadMSBData(string filepath)
        {
            string folder = Path.GetDirectoryName(filepath);
            string fileName = Path.GetFileName(filepath);

            var pred = new Predicate<SelSpectrum>(x => (x.Folder == folder && x.FileName == fileName));
            int find = _selSpectrum.FindIndex(pred);

            if (find >= 0)
                return -1;

            FileFactoryWrapper factory = FileFactoryWrapper.getInstance();
            SampleSetWrapper set = factory.openFile(filepath);

            if (set == null) return -1;

            for (int num = 0; num < set.getNumberOfSamples(); num++)
            {
                FileData _tmpFileNameData = new FileData(num + 1, folder, fileName);

                SampleWrapper sample = set.getSample((uint)num);
                sample.openSample();
                Debug.WriteLine(sample.getSampleSet().getFileName());

                DataGroupNodeWrapper root = sample.getRootDataGroupNode();
                List<ReadSpectrumData> listReadSpec = new List<ReadSpectrumData>();
                List<uint> DescendantList = new List<uint>();
                ReadSpectrum(root, 0, 0, ref listReadSpec, DescendantList);

                for (uint n = 0; n < listReadSpec.Count; n++)
                {
                    SpectrumWrapper spec = listReadSpec[(int)n].Spec;

                    string precursor = "";
                    if (spec.getMsStage() == 1)
                    {
                        precursor = "-----";
                    }
                    else if (spec.getMsStage() == 2)
                    {
                        precursor = spec.getPrecursor().ToString();
                    }
                    else
                    {
                        precursor = "\"" + spec.getPrecursor(1).ToString();
                        for (int pcnt = 2; pcnt < spec.getMsStage(); pcnt++)
                        {
                            precursor += ("," + spec.getPrecursor(pcnt).ToString());
                        }
                        precursor += "\"";
                    }
                    SelSpectrum selSpec = new SelSpectrum(num + 1,
                        folder, fileName, spec.getName(), spec.getRt().ToString(), precursor,
                        n, listReadSpec[(int)n].DescendantList, listReadSpec[(int)n].Descendant,
                        listReadSpec[(int)n].ChildNum, listReadSpec[(int)n].SpecNum);

                    selSpec.TotalSpecNum = n;

                    _selSpectrum.Add(selSpec);

                    SpectrumData tmpSpectrumNameData = new SpectrumData();
                    tmpSpectrumNameData.SpectrumName = spec.getName();
                    tmpSpectrumNameData.RT = spec.getRt();

                    if (spec.getMsStage() == 1)
                    {
                        tmpSpectrumNameData.Precusor = "-----";
                    }
                    else if (spec.getMsStage() == 2)
                    {
                        tmpSpectrumNameData.Precusor = spec.getPrecursor().ToString();
                    }
                    else
                    {
                        tmpSpectrumNameData.Precusor = "\"" + spec.getPrecursor(1).ToString(); ;
                        for (int pcnt = 2; pcnt < spec.getMsStage(); pcnt++)
                        {
                            tmpSpectrumNameData.Precusor = tmpSpectrumNameData.Precusor + "," + spec.getPrecursor(pcnt).ToString();
                        }
                        tmpSpectrumNameData.Precusor = tmpSpectrumNameData.Precusor + "\"";
                    }

                    _tmpFileNameData.SpectrumNameDataList.Add(tmpSpectrumNameData);
                    _tmpFileNameData.SpectrumNameList.Add(spec.getName());
                }

                _fileNameDataList.Add(_tmpFileNameData);

                _bindFileName.Add("[" + _tmpFileNameData.SampleTabNo.ToString() + "] " + _tmpFileNameData.FileName);

                _bindFileNameLastRow.Insert(_bindFileNameLastRow.Count - 1, "["
                    + _tmpFileNameData.SampleTabNo.ToString() + "] " + _tmpFileNameData.FileName);
            }

            factory.closeFile(set);
            return 0;
        }




        #endregion

        #region --- Private Methods -------------------------------------

        /// <summary>
        /// Read Spectrum
        /// </summary>
        /// <param name="node">DataGroupNodeWrapper</param>
        /// <param name="descendant">descendant of node(child or grandchild.....</param>
        /// <param name="childNum">child number</param>
        /// <param name="listReadSpec">ReadSpectrumData List</param>
        /// <param name="descendantList">descendant List</param>
        private void ReadSpectrum(DataGroupNodeWrapper node, uint descendant,
            uint childNum, ref List<ReadSpectrumData> listReadSpec, List<uint> descendantList)
        {
            uint numSpec = node.getNumberOfSpectra();
            for (uint n = 0; n < numSpec; n++)
            {
                SpectrumWrapper spec = node.getSpectrum(n);
                ReadSpectrumData tmpData = new ReadSpectrumData(spec, descendantList, descendant, childNum, n);
                listReadSpec.Add(tmpData);
            }

            uint numChild = node.getNumberOfChildren();
            for (uint n = 0; n < numChild; n++)
            {
                DataGroupNodeWrapper child = node.getChild(n);
                if (child != null)
                {
                    List<uint> tmpDescendant = new List<uint>();
                    for (int cnt = 0; cnt < descendantList.Count; cnt++)
                    {
                        tmpDescendant.Add(descendantList[cnt]);
                    }
                    tmpDescendant.Add(n);
                    ReadSpectrum(child, descendant + 1, n, ref listReadSpec, tmpDescendant);
                }
                else
                {
                    Debug.Assert(false);
                }
            }

        }

        #endregion
    }
}
