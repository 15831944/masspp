#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Data.dll>
#using <System.Xml.dll>

using namespace System::Security::Permissions;
[assembly:SecurityPermissionAttribute(SecurityAction::RequestMinimum, SkipVerification=false)];
// 
// このソース コードは xsd によって自動生成されました。Version=4.0.30319.1 です。
// 
using namespace System;
ref class NewDataSet;


/// <summary>
///Represents a strongly typed in-memory cache of data.
///</summary>
[System::Serializable, 
System::ComponentModel::DesignerCategoryAttribute(L"code"), 
System::ComponentModel::ToolboxItem(true), 
System::Xml::Serialization::XmlSchemaProviderAttribute(L"GetTypedDataSetSchema"), 
System::Xml::Serialization::XmlRootAttribute(L"NewDataSet"), 
System::ComponentModel::Design::HelpKeywordAttribute(L"vs.data.DataSet")]
public ref class NewDataSet : public ::System::Data::DataSet {
    public : ref class operationsDataTable;
    public : ref class operationDataTable;
    public : ref class dataDataTable;
    public : ref class operationsRow;
    public : ref class operationRow;
    public : ref class dataRow;
    public : ref class operationsRowChangeEvent;
    public : ref class operationRowChangeEvent;
    public : ref class dataRowChangeEvent;
    
    private: NewDataSet::operationsDataTable^  tableoperations;
    
    private: NewDataSet::operationDataTable^  tableoperation;
    
    private: NewDataSet::dataDataTable^  tabledata;
    
    private: ::System::Data::DataRelation^  relationoperations_operation;
    
    private: ::System::Data::DataRelation^  relationoperation_data;
    
    private: ::System::Data::SchemaSerializationMode _schemaSerializationMode;
    
    public : [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    delegate System::Void operationsRowChangeEventHandler(::System::Object^  sender, NewDataSet::operationsRowChangeEvent^  e);
    
    public : [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    delegate System::Void operationRowChangeEventHandler(::System::Object^  sender, NewDataSet::operationRowChangeEvent^  e);
    
    public : [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    delegate System::Void dataRowChangeEventHandler(::System::Object^  sender, NewDataSet::dataRowChangeEvent^  e);
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    NewDataSet();
    protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    NewDataSet(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context);
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
    System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
    System::ComponentModel::Browsable(false), 
    System::ComponentModel::DesignerSerializationVisibility(::System::ComponentModel::DesignerSerializationVisibility::Content)]
    property NewDataSet::operationsDataTable^  operations {
        NewDataSet::operationsDataTable^  get();
    }
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
    System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
    System::ComponentModel::Browsable(false), 
    System::ComponentModel::DesignerSerializationVisibility(::System::ComponentModel::DesignerSerializationVisibility::Content)]
    property NewDataSet::operationDataTable^  operation {
        NewDataSet::operationDataTable^  get();
    }
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
    System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
    System::ComponentModel::Browsable(false), 
    System::ComponentModel::DesignerSerializationVisibility(::System::ComponentModel::DesignerSerializationVisibility::Content)]
    property NewDataSet::dataDataTable^  data {
        NewDataSet::dataDataTable^  get();
    }
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
    System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
    System::ComponentModel::BrowsableAttribute(true), 
    System::ComponentModel::DesignerSerializationVisibilityAttribute(::System::ComponentModel::DesignerSerializationVisibility::Visible)]
    virtual property ::System::Data::SchemaSerializationMode SchemaSerializationMode {
        ::System::Data::SchemaSerializationMode get() override;
        System::Void set(::System::Data::SchemaSerializationMode value) override;
    }
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
    System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
    System::ComponentModel::DesignerSerializationVisibilityAttribute(::System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property ::System::Data::DataTableCollection^  Tables {
        ::System::Data::DataTableCollection^  get() new;
    }
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
    System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
    System::ComponentModel::DesignerSerializationVisibilityAttribute(::System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property ::System::Data::DataRelationCollection^  Relations {
        ::System::Data::DataRelationCollection^  get() new;
    }
    
    protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    virtual ::System::Void InitializeDerivedDataSet() override;
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    virtual ::System::Data::DataSet^  Clone() override;
    
    protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    virtual ::System::Boolean ShouldSerializeTables() override;
    
    protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    virtual ::System::Boolean ShouldSerializeRelations() override;
    
    protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    virtual ::System::Void ReadXmlSerializable(::System::Xml::XmlReader^  reader) override;
    
    protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    virtual ::System::Xml::Schema::XmlSchema^  GetSchemaSerializable() override;
    
    internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ::System::Void InitVars();
    
    internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ::System::Void InitVars(::System::Boolean initTable);
    
    private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ::System::Void InitClass();
    
    private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ::System::Boolean ShouldSerializeoperations();
    
    private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ::System::Boolean ShouldSerializeoperation();
    
    private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ::System::Boolean ShouldSerializedata();
    
    private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ::System::Void SchemaChanged(::System::Object^  sender, ::System::ComponentModel::CollectionChangeEventArgs^  e);
    
    public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    static ::System::Xml::Schema::XmlSchemaComplexType^  GetTypedDataSetSchema(::System::Xml::Schema::XmlSchemaSet^  xs);
    
    public : /// <summary>
///Represents the strongly named DataTable class.
///</summary>
    [System::Serializable, 
    System::Xml::Serialization::XmlSchemaProviderAttribute(L"GetTypedTableSchema")]
    ref class operationsDataTable : public ::System::Data::DataTable, public ::System::Collections::IEnumerable {
        
        private: ::System::Data::DataColumn^  columntype;
        
        private: ::System::Data::DataColumn^  columnstate;
        
        private: ::System::Data::DataColumn^  columnoperations_Id;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationsRowChangeEventHandler^  operationsRowChanging;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationsRowChangeEventHandler^  operationsRowChanged;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationsRowChangeEventHandler^  operationsRowDeleting;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationsRowChangeEventHandler^  operationsRowDeleted;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationsDataTable();
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationsDataTable(::System::Data::DataTable^  table);
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationsDataTable(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  typeColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  stateColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  operations_IdColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
        System::ComponentModel::Browsable(false)]
        property ::System::Int32 Count {
            ::System::Int32 get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::operationsRow^  default [::System::Int32 ] {
            NewDataSet::operationsRow^  get(::System::Int32 index);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void AddoperationsRow(NewDataSet::operationsRow^  row);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        NewDataSet::operationsRow^  AddoperationsRow(System::String^  type, System::Int64 state);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Collections::IEnumerator^  GetEnumerator();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataTable^  Clone() override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataTable^  CreateInstance() override;
        
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void InitVars();
        
        private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void InitClass();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        NewDataSet::operationsRow^  NewoperationsRow();
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataRow^  NewRowFromBuilder(::System::Data::DataRowBuilder^  builder) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Type^  GetRowType() override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowChanged(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowChanging(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowDeleted(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowDeleting(::System::Data::DataRowChangeEventArgs^  e) override;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void RemoveoperationsRow(NewDataSet::operationsRow^  row);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        static ::System::Xml::Schema::XmlSchemaComplexType^  GetTypedTableSchema(::System::Xml::Schema::XmlSchemaSet^  xs);
    };
    
    public : /// <summary>
///Represents the strongly named DataTable class.
///</summary>
    [System::Serializable, 
    System::Xml::Serialization::XmlSchemaProviderAttribute(L"GetTypedTableSchema")]
    ref class operationDataTable : public ::System::Data::DataTable, public ::System::Collections::IEnumerable {
        
        private: ::System::Data::DataColumn^  columnname;
        
        private: ::System::Data::DataColumn^  column_short_name;
        
        private: ::System::Data::DataColumn^  columndesc;
        
        private: ::System::Data::DataColumn^  columnparam;
        
        private: ::System::Data::DataColumn^  columnoperation_Id;
        
        private: ::System::Data::DataColumn^  columnoperations_Id;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationRowChangeEventHandler^  operationRowChanging;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationRowChangeEventHandler^  operationRowChanged;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationRowChangeEventHandler^  operationRowDeleting;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::operationRowChangeEventHandler^  operationRowDeleted;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationDataTable();
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationDataTable(::System::Data::DataTable^  table);
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationDataTable(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  nameColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  _short_nameColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  descColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  paramColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  operation_IdColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  operations_IdColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
        System::ComponentModel::Browsable(false)]
        property ::System::Int32 Count {
            ::System::Int32 get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::operationRow^  default [::System::Int32 ] {
            NewDataSet::operationRow^  get(::System::Int32 index);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void AddoperationRow(NewDataSet::operationRow^  row);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        NewDataSet::operationRow^  AddoperationRow(System::String^  name, System::String^  _short_name, System::String^  desc, 
                    System::String^  param, NewDataSet::operationsRow^  parentoperationsRowByoperations_operation);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Collections::IEnumerator^  GetEnumerator();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataTable^  Clone() override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataTable^  CreateInstance() override;
        
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void InitVars();
        
        private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void InitClass();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        NewDataSet::operationRow^  NewoperationRow();
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataRow^  NewRowFromBuilder(::System::Data::DataRowBuilder^  builder) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Type^  GetRowType() override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowChanged(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowChanging(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowDeleted(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowDeleting(::System::Data::DataRowChangeEventArgs^  e) override;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void RemoveoperationRow(NewDataSet::operationRow^  row);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        static ::System::Xml::Schema::XmlSchemaComplexType^  GetTypedTableSchema(::System::Xml::Schema::XmlSchemaSet^  xs);
    };
    
    public : /// <summary>
///Represents the strongly named DataTable class.
///</summary>
    [System::Serializable, 
    System::Xml::Serialization::XmlSchemaProviderAttribute(L"GetTypedTableSchema")]
    ref class dataDataTable : public ::System::Data::DataTable, public ::System::Collections::IEnumerable {
        
        private: ::System::Data::DataColumn^  columnbefore;
        
        private: ::System::Data::DataColumn^  columnafter;
        
        private: ::System::Data::DataColumn^  columnoperation_Id;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::dataRowChangeEventHandler^  dataRowChanging;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::dataRowChangeEventHandler^  dataRowChanged;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::dataRowChangeEventHandler^  dataRowDeleting;
        
        public: [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        event NewDataSet::dataRowChangeEventHandler^  dataRowDeleted;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        dataDataTable();
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        dataDataTable(::System::Data::DataTable^  table);
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        dataDataTable(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  beforeColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  afterColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataColumn^  operation_IdColumn {
            ::System::Data::DataColumn^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0"), 
        System::ComponentModel::Browsable(false)]
        property ::System::Int32 Count {
            ::System::Int32 get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::dataRow^  default [::System::Int32 ] {
            NewDataSet::dataRow^  get(::System::Int32 index);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void AdddataRow(NewDataSet::dataRow^  row);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        NewDataSet::dataRow^  AdddataRow(cli::array< System::Byte >^  before, cli::array< System::Byte >^  after, NewDataSet::operationRow^  parentoperationRowByoperation_data);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Collections::IEnumerator^  GetEnumerator();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataTable^  Clone() override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataTable^  CreateInstance() override;
        
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void InitVars();
        
        private: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void InitClass();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        NewDataSet::dataRow^  NewdataRow();
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Data::DataRow^  NewRowFromBuilder(::System::Data::DataRowBuilder^  builder) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Type^  GetRowType() override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowChanged(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowChanging(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowDeleted(::System::Data::DataRowChangeEventArgs^  e) override;
        
        protected: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        virtual ::System::Void OnRowDeleting(::System::Data::DataRowChangeEventArgs^  e) override;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void RemovedataRow(NewDataSet::dataRow^  row);
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        static ::System::Xml::Schema::XmlSchemaComplexType^  GetTypedTableSchema(::System::Xml::Schema::XmlSchemaSet^  xs);
    };
    
    public : /// <summary>
///Represents strongly named DataRow class.
///</summary>
    ref class operationsRow : public ::System::Data::DataRow {
        
        private: NewDataSet::operationsDataTable^  tableoperations;
        
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationsRow(::System::Data::DataRowBuilder^  rb);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::String^  type {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::Int64 state {
            System::Int64 get();
            System::Void set(System::Int64 value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::Int32 operations_Id {
            System::Int32 get();
            System::Void set(System::Int32 value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Boolean IstypeNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void SettypeNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Boolean IsstateNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void SetstateNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        cli::array< NewDataSet::operationRow^  >^  GetoperationRows();
    };
    
    public : /// <summary>
///Represents strongly named DataRow class.
///</summary>
    ref class operationRow : public ::System::Data::DataRow {
        
        private: NewDataSet::operationDataTable^  tableoperation;
        
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationRow(::System::Data::DataRowBuilder^  rb);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::String^  name {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::String^  _short_name {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::String^  desc {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::String^  param {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::Int32 operation_Id {
            System::Int32 get();
            System::Void set(System::Int32 value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::Int32 operations_Id {
            System::Int32 get();
            System::Void set(System::Int32 value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::operationsRow^  operationsRow {
            NewDataSet::operationsRow^  get();
            System::Void set(NewDataSet::operationsRow^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Boolean IsdescNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void SetdescNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Boolean Isoperations_IdNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void Setoperations_IdNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        cli::array< NewDataSet::dataRow^  >^  GetdataRows();
    };
    
    public : /// <summary>
///Represents strongly named DataRow class.
///</summary>
    ref class dataRow : public ::System::Data::DataRow {
        
        private: NewDataSet::dataDataTable^  tabledata;
        
        internal: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        dataRow(::System::Data::DataRowBuilder^  rb);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property cli::array< System::Byte >^  before {
            cli::array< System::Byte >^  get();
            System::Void set(cli::array< System::Byte >^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property cli::array< System::Byte >^  after {
            cli::array< System::Byte >^  get();
            System::Void set(cli::array< System::Byte >^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property System::Int32 operation_Id {
            System::Int32 get();
            System::Void set(System::Int32 value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::operationRow^  operationRow {
            NewDataSet::operationRow^  get();
            System::Void set(NewDataSet::operationRow^  value);
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Boolean Isoperation_IdNull();
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        ::System::Void Setoperation_IdNull();
    };
    
    public : /// <summary>
///Row event argument class
///</summary>
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ref class operationsRowChangeEvent : public ::System::EventArgs {
        
        private: NewDataSet::operationsRow^  eventRow;
        
        private: ::System::Data::DataRowAction eventAction;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationsRowChangeEvent(NewDataSet::operationsRow^  row, ::System::Data::DataRowAction action);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::operationsRow^  Row {
            NewDataSet::operationsRow^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataRowAction Action {
            ::System::Data::DataRowAction get();
        }
    };
    
    public : /// <summary>
///Row event argument class
///</summary>
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ref class operationRowChangeEvent : public ::System::EventArgs {
        
        private: NewDataSet::operationRow^  eventRow;
        
        private: ::System::Data::DataRowAction eventAction;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        operationRowChangeEvent(NewDataSet::operationRow^  row, ::System::Data::DataRowAction action);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::operationRow^  Row {
            NewDataSet::operationRow^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataRowAction Action {
            ::System::Data::DataRowAction get();
        }
    };
    
    public : /// <summary>
///Row event argument class
///</summary>
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
    ref class dataRowChangeEvent : public ::System::EventArgs {
        
        private: NewDataSet::dataRow^  eventRow;
        
        private: ::System::Data::DataRowAction eventAction;
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute]
        [System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        dataRowChangeEvent(NewDataSet::dataRow^  row, ::System::Data::DataRowAction action);
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property NewDataSet::dataRow^  Row {
            NewDataSet::dataRow^  get();
        }
        
        public: [System::Diagnostics::DebuggerNonUserCodeAttribute, 
        System::CodeDom::Compiler::GeneratedCodeAttribute(L"System.Data.Design.TypedDataSetGenerator", L"4.0.0.0")]
        property ::System::Data::DataRowAction Action {
            ::System::Data::DataRowAction get();
        }
    };
};


inline NewDataSet::NewDataSet() {
    this->BeginInit();
    this->InitClass();
    ::System::ComponentModel::CollectionChangeEventHandler^  schemaChangedHandler = gcnew ::System::ComponentModel::CollectionChangeEventHandler(this, &NewDataSet::SchemaChanged);
    __super::Tables->CollectionChanged += schemaChangedHandler;
    __super::Relations->CollectionChanged += schemaChangedHandler;
    this->EndInit();
}

inline NewDataSet::NewDataSet(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context) : 
        ::System::Data::DataSet(info, context, false) {
    if (this->IsBinarySerialized(info, context) == true) {
        this->InitVars(false);
        ::System::ComponentModel::CollectionChangeEventHandler^  schemaChangedHandler1 = gcnew ::System::ComponentModel::CollectionChangeEventHandler(this, &NewDataSet::SchemaChanged);
        this->Tables->CollectionChanged += schemaChangedHandler1;
        this->Relations->CollectionChanged += schemaChangedHandler1;
        return;
    }
    ::System::String^  strSchema = (cli::safe_cast<::System::String^  >(info->GetValue(L"XmlSchema", ::System::String::typeid)));
    if (this->DetermineSchemaSerializationMode(info, context) == ::System::Data::SchemaSerializationMode::IncludeSchema) {
        ::System::Data::DataSet^  ds = (gcnew ::System::Data::DataSet());
        ds->ReadXmlSchema((gcnew ::System::Xml::XmlTextReader((gcnew ::System::IO::StringReader(strSchema)))));
        if (ds->Tables[L"operations"] != nullptr) {
            __super::Tables->Add((gcnew NewDataSet::operationsDataTable(ds->Tables[L"operations"])));
        }
        if (ds->Tables[L"operation"] != nullptr) {
            __super::Tables->Add((gcnew NewDataSet::operationDataTable(ds->Tables[L"operation"])));
        }
        if (ds->Tables[L"data"] != nullptr) {
            __super::Tables->Add((gcnew NewDataSet::dataDataTable(ds->Tables[L"data"])));
        }
        this->DataSetName = ds->DataSetName;
        this->Prefix = ds->Prefix;
        this->Namespace = ds->Namespace;
        this->Locale = ds->Locale;
        this->CaseSensitive = ds->CaseSensitive;
        this->EnforceConstraints = ds->EnforceConstraints;
        this->Merge(ds, false, ::System::Data::MissingSchemaAction::Add);
        this->InitVars();
    }
    else {
        this->ReadXmlSchema((gcnew ::System::Xml::XmlTextReader((gcnew ::System::IO::StringReader(strSchema)))));
    }
    this->GetSerializationData(info, context);
    ::System::ComponentModel::CollectionChangeEventHandler^  schemaChangedHandler = gcnew ::System::ComponentModel::CollectionChangeEventHandler(this, &NewDataSet::SchemaChanged);
    __super::Tables->CollectionChanged += schemaChangedHandler;
    this->Relations->CollectionChanged += schemaChangedHandler;
}

inline NewDataSet::operationsDataTable^  NewDataSet::operations::get() {
    return this->tableoperations;
}

inline NewDataSet::operationDataTable^  NewDataSet::operation::get() {
    return this->tableoperation;
}

inline NewDataSet::dataDataTable^  NewDataSet::data::get() {
    return this->tabledata;
}

inline ::System::Data::SchemaSerializationMode NewDataSet::SchemaSerializationMode::get() {
    return this->_schemaSerializationMode;
}
inline System::Void NewDataSet::SchemaSerializationMode::set(::System::Data::SchemaSerializationMode value) {
    this->_schemaSerializationMode = __identifier(value);
}

inline ::System::Data::DataTableCollection^  NewDataSet::Tables::get() {
    return __super::Tables;
}

inline ::System::Data::DataRelationCollection^  NewDataSet::Relations::get() {
    return __super::Relations;
}

inline ::System::Void NewDataSet::InitializeDerivedDataSet() {
    this->BeginInit();
    this->InitClass();
    this->EndInit();
}

inline ::System::Data::DataSet^  NewDataSet::Clone() {
    NewDataSet^  cln = (cli::safe_cast<NewDataSet^  >(__super::Clone()));
    cln->InitVars();
    cln->SchemaSerializationMode = this->SchemaSerializationMode;
    return cln;
}

inline ::System::Boolean NewDataSet::ShouldSerializeTables() {
    return false;
}

inline ::System::Boolean NewDataSet::ShouldSerializeRelations() {
    return false;
}

inline ::System::Void NewDataSet::ReadXmlSerializable(::System::Xml::XmlReader^  reader) {
    if (this->DetermineSchemaSerializationMode(reader) == ::System::Data::SchemaSerializationMode::IncludeSchema) {
        this->Reset();
        ::System::Data::DataSet^  ds = (gcnew ::System::Data::DataSet());
        ds->ReadXml(reader);
        if (ds->Tables[L"operations"] != nullptr) {
            __super::Tables->Add((gcnew NewDataSet::operationsDataTable(ds->Tables[L"operations"])));
        }
        if (ds->Tables[L"operation"] != nullptr) {
            __super::Tables->Add((gcnew NewDataSet::operationDataTable(ds->Tables[L"operation"])));
        }
        if (ds->Tables[L"data"] != nullptr) {
            __super::Tables->Add((gcnew NewDataSet::dataDataTable(ds->Tables[L"data"])));
        }
        this->DataSetName = ds->DataSetName;
        this->Prefix = ds->Prefix;
        this->Namespace = ds->Namespace;
        this->Locale = ds->Locale;
        this->CaseSensitive = ds->CaseSensitive;
        this->EnforceConstraints = ds->EnforceConstraints;
        this->Merge(ds, false, ::System::Data::MissingSchemaAction::Add);
        this->InitVars();
    }
    else {
        this->ReadXml(reader);
        this->InitVars();
    }
}

inline ::System::Xml::Schema::XmlSchema^  NewDataSet::GetSchemaSerializable() {
    ::System::IO::MemoryStream^  stream = (gcnew ::System::IO::MemoryStream());
    this->WriteXmlSchema((gcnew ::System::Xml::XmlTextWriter(stream, nullptr)));
    stream->Position = 0;
    return ::System::Xml::Schema::XmlSchema::Read((gcnew ::System::Xml::XmlTextReader(stream)), nullptr);
}

inline ::System::Void NewDataSet::InitVars() {
    this->InitVars(true);
}

inline ::System::Void NewDataSet::InitVars(::System::Boolean initTable) {
    this->tableoperations = (cli::safe_cast<NewDataSet::operationsDataTable^  >(__super::Tables[L"operations"]));
    if (initTable == true) {
        if (this->tableoperations != nullptr) {
            this->tableoperations->InitVars();
        }
    }
    this->tableoperation = (cli::safe_cast<NewDataSet::operationDataTable^  >(__super::Tables[L"operation"]));
    if (initTable == true) {
        if (this->tableoperation != nullptr) {
            this->tableoperation->InitVars();
        }
    }
    this->tabledata = (cli::safe_cast<NewDataSet::dataDataTable^  >(__super::Tables[L"data"]));
    if (initTable == true) {
        if (this->tabledata != nullptr) {
            this->tabledata->InitVars();
        }
    }
    this->relationoperations_operation = this->Relations[L"operations_operation"];
    this->relationoperation_data = this->Relations[L"operation_data"];
}

inline ::System::Void NewDataSet::InitClass() {
    this->DataSetName = L"NewDataSet";
    this->Prefix = L"";
    this->Namespace = L"http://xml.first-ms3d.jp/masspp/operations";
    this->Locale = (gcnew ::System::Globalization::CultureInfo(L""));
    this->EnforceConstraints = true;
    this->SchemaSerializationMode = ::System::Data::SchemaSerializationMode::IncludeSchema;
    this->tableoperations = (gcnew NewDataSet::operationsDataTable());
    __super::Tables->Add(this->tableoperations);
    this->tableoperation = (gcnew NewDataSet::operationDataTable());
    __super::Tables->Add(this->tableoperation);
    this->tabledata = (gcnew NewDataSet::dataDataTable());
    __super::Tables->Add(this->tabledata);
    ::System::Data::ForeignKeyConstraint^  fkc;
    fkc = (gcnew ::System::Data::ForeignKeyConstraint(L"operations_operation", gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tableoperations->operations_IdColumn}, 
        gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tableoperation->operations_IdColumn}));
    this->tableoperation->Constraints->Add(fkc);
    fkc->AcceptRejectRule = ::System::Data::AcceptRejectRule::None;
    fkc->DeleteRule = ::System::Data::Rule::Cascade;
    fkc->UpdateRule = ::System::Data::Rule::Cascade;
    fkc = (gcnew ::System::Data::ForeignKeyConstraint(L"operation_data", gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tableoperation->operation_IdColumn}, 
        gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tabledata->operation_IdColumn}));
    this->tabledata->Constraints->Add(fkc);
    fkc->AcceptRejectRule = ::System::Data::AcceptRejectRule::None;
    fkc->DeleteRule = ::System::Data::Rule::Cascade;
    fkc->UpdateRule = ::System::Data::Rule::Cascade;
    this->relationoperations_operation = (gcnew ::System::Data::DataRelation(L"operations_operation", gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tableoperations->operations_IdColumn}, 
        gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tableoperation->operations_IdColumn}, false));
    this->relationoperations_operation->Nested = true;
    this->Relations->Add(this->relationoperations_operation);
    this->relationoperation_data = (gcnew ::System::Data::DataRelation(L"operation_data", gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tableoperation->operation_IdColumn}, 
        gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->tabledata->operation_IdColumn}, false));
    this->relationoperation_data->Nested = true;
    this->Relations->Add(this->relationoperation_data);
}

inline ::System::Boolean NewDataSet::ShouldSerializeoperations() {
    return false;
}

inline ::System::Boolean NewDataSet::ShouldSerializeoperation() {
    return false;
}

inline ::System::Boolean NewDataSet::ShouldSerializedata() {
    return false;
}

inline ::System::Void NewDataSet::SchemaChanged(::System::Object^  sender, ::System::ComponentModel::CollectionChangeEventArgs^  e) {
    if (e->Action == ::System::ComponentModel::CollectionChangeAction::Remove) {
        this->InitVars();
    }
}

inline ::System::Xml::Schema::XmlSchemaComplexType^  NewDataSet::GetTypedDataSetSchema(::System::Xml::Schema::XmlSchemaSet^  xs) {
    NewDataSet^  ds = (gcnew NewDataSet());
    ::System::Xml::Schema::XmlSchemaComplexType^  type = (gcnew ::System::Xml::Schema::XmlSchemaComplexType());
    ::System::Xml::Schema::XmlSchemaSequence^  sequence = (gcnew ::System::Xml::Schema::XmlSchemaSequence());
    ::System::Xml::Schema::XmlSchemaAny^  any = (gcnew ::System::Xml::Schema::XmlSchemaAny());
    any->Namespace = ds->Namespace;
    sequence->Items->Add(any);
    type->Particle = sequence;
    ::System::Xml::Schema::XmlSchema^  dsSchema = ds->GetSchemaSerializable();
    if (xs->Contains(dsSchema->TargetNamespace)) {
        ::System::IO::MemoryStream^  s1 = (gcnew ::System::IO::MemoryStream());
        ::System::IO::MemoryStream^  s2 = (gcnew ::System::IO::MemoryStream());
        try {
            ::System::Xml::Schema::XmlSchema^  schema = nullptr;
            dsSchema->Write(s1);
            for (            ::System::Collections::IEnumerator^  schemas = xs->Schemas(dsSchema->TargetNamespace)->GetEnumerator(); schemas->MoveNext();             ) {
                schema = (cli::safe_cast<::System::Xml::Schema::XmlSchema^  >(schemas->Current));
                s2->SetLength(0);
                schema->Write(s2);
                if (s1->Length == s2->Length) {
                    s1->Position = 0;
                    s2->Position = 0;
                    for (                    ; ((s1->Position != s1->Length) 
                                && (s1->ReadByte() == s2->ReadByte()));                     ) {
                        ;
                    }
                    if (s1->Position == s1->Length) {
                        return type;
                    }
                }
            }
        }
        finally {
            if (s1 != nullptr) {
                s1->Close();
            }
            if (s2 != nullptr) {
                s2->Close();
            }
        }
    }
    xs->Add(dsSchema);
    return type;
}


inline NewDataSet::operationsDataTable::operationsDataTable() {
    this->TableName = L"operations";
    this->BeginInit();
    this->InitClass();
    this->EndInit();
}

inline NewDataSet::operationsDataTable::operationsDataTable(::System::Data::DataTable^  table) {
    this->TableName = table->TableName;
    if (table->CaseSensitive != table->DataSet->CaseSensitive) {
        this->CaseSensitive = table->CaseSensitive;
    }
    if (table->Locale->ToString() != table->DataSet->Locale->ToString()) {
        this->Locale = table->Locale;
    }
    if (table->Namespace != table->DataSet->Namespace) {
        this->Namespace = table->Namespace;
    }
    this->Prefix = table->Prefix;
    this->MinimumCapacity = table->MinimumCapacity;
}

inline NewDataSet::operationsDataTable::operationsDataTable(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context) : 
        ::System::Data::DataTable(info, context) {
    this->InitVars();
}

inline ::System::Data::DataColumn^  NewDataSet::operationsDataTable::typeColumn::get() {
    return this->columntype;
}

inline ::System::Data::DataColumn^  NewDataSet::operationsDataTable::stateColumn::get() {
    return this->columnstate;
}

inline ::System::Data::DataColumn^  NewDataSet::operationsDataTable::operations_IdColumn::get() {
    return this->columnoperations_Id;
}

inline ::System::Int32 NewDataSet::operationsDataTable::Count::get() {
    return this->Rows->Count;
}

inline NewDataSet::operationsRow^  NewDataSet::operationsDataTable::default::get(::System::Int32 index) {
    return (cli::safe_cast<NewDataSet::operationsRow^  >(this->Rows[index]));
}

inline ::System::Void NewDataSet::operationsDataTable::AddoperationsRow(NewDataSet::operationsRow^  row) {
    this->Rows->Add(row);
}

inline NewDataSet::operationsRow^  NewDataSet::operationsDataTable::AddoperationsRow(System::String^  type, System::Int64 state) {
    NewDataSet::operationsRow^  rowoperationsRow = (cli::safe_cast<NewDataSet::operationsRow^  >(this->NewRow()));
    cli::array< ::System::Object^  >^  columnValuesArray = gcnew cli::array< ::System::Object^  >(3) {type, state, nullptr};
    rowoperationsRow->ItemArray = columnValuesArray;
    this->Rows->Add(rowoperationsRow);
    return rowoperationsRow;
}

inline ::System::Collections::IEnumerator^  NewDataSet::operationsDataTable::GetEnumerator() {
    return this->Rows->GetEnumerator();
}

inline ::System::Data::DataTable^  NewDataSet::operationsDataTable::Clone() {
    NewDataSet::operationsDataTable^  cln = (cli::safe_cast<NewDataSet::operationsDataTable^  >(__super::Clone()));
    cln->InitVars();
    return cln;
}

inline ::System::Data::DataTable^  NewDataSet::operationsDataTable::CreateInstance() {
    return (gcnew NewDataSet::operationsDataTable());
}

inline ::System::Void NewDataSet::operationsDataTable::InitVars() {
    this->columntype = __super::Columns[L"type"];
    this->columnstate = __super::Columns[L"state"];
    this->columnoperations_Id = __super::Columns[L"operations_Id"];
}

inline ::System::Void NewDataSet::operationsDataTable::InitClass() {
    this->columntype = (gcnew ::System::Data::DataColumn(L"type", ::System::String::typeid, nullptr, ::System::Data::MappingType::Attribute));
    __super::Columns->Add(this->columntype);
    this->columnstate = (gcnew ::System::Data::DataColumn(L"state", ::System::Int64::typeid, nullptr, ::System::Data::MappingType::Attribute));
    __super::Columns->Add(this->columnstate);
    this->columnoperations_Id = (gcnew ::System::Data::DataColumn(L"operations_Id", ::System::Int32::typeid, nullptr, ::System::Data::MappingType::Hidden));
    __super::Columns->Add(this->columnoperations_Id);
    this->Constraints->Add((gcnew ::System::Data::UniqueConstraint(L"Constraint1", gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->columnoperations_Id}, 
            true)));
    this->columntype->Namespace = L"";
    this->columnstate->Namespace = L"";
    this->columnoperations_Id->AutoIncrement = true;
    this->columnoperations_Id->AllowDBNull = false;
    this->columnoperations_Id->Unique = true;
}

inline NewDataSet::operationsRow^  NewDataSet::operationsDataTable::NewoperationsRow() {
    return (cli::safe_cast<NewDataSet::operationsRow^  >(this->NewRow()));
}

inline ::System::Data::DataRow^  NewDataSet::operationsDataTable::NewRowFromBuilder(::System::Data::DataRowBuilder^  builder) {
    return (gcnew NewDataSet::operationsRow(builder));
}

inline ::System::Type^  NewDataSet::operationsDataTable::GetRowType() {
    return NewDataSet::operationsRow::typeid;
}

inline ::System::Void NewDataSet::operationsDataTable::OnRowChanged(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowChanged(e);
    {
        this->operationsRowChanged(this, (gcnew NewDataSet::operationsRowChangeEvent((cli::safe_cast<NewDataSet::operationsRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationsDataTable::OnRowChanging(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowChanging(e);
    {
        this->operationsRowChanging(this, (gcnew NewDataSet::operationsRowChangeEvent((cli::safe_cast<NewDataSet::operationsRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationsDataTable::OnRowDeleted(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowDeleted(e);
    {
        this->operationsRowDeleted(this, (gcnew NewDataSet::operationsRowChangeEvent((cli::safe_cast<NewDataSet::operationsRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationsDataTable::OnRowDeleting(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowDeleting(e);
    {
        this->operationsRowDeleting(this, (gcnew NewDataSet::operationsRowChangeEvent((cli::safe_cast<NewDataSet::operationsRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationsDataTable::RemoveoperationsRow(NewDataSet::operationsRow^  row) {
    this->Rows->Remove(row);
}

inline ::System::Xml::Schema::XmlSchemaComplexType^  NewDataSet::operationsDataTable::GetTypedTableSchema(::System::Xml::Schema::XmlSchemaSet^  xs) {
    ::System::Xml::Schema::XmlSchemaComplexType^  type = (gcnew ::System::Xml::Schema::XmlSchemaComplexType());
    ::System::Xml::Schema::XmlSchemaSequence^  sequence = (gcnew ::System::Xml::Schema::XmlSchemaSequence());
    NewDataSet^  ds = (gcnew NewDataSet());
    ::System::Xml::Schema::XmlSchemaAny^  any1 = (gcnew ::System::Xml::Schema::XmlSchemaAny());
    any1->Namespace = L"http://www.w3.org/2001/XMLSchema";
    any1->MinOccurs = ::System::Decimal(0);
    any1->MaxOccurs = ::System::Decimal::MaxValue;
    any1->ProcessContents = ::System::Xml::Schema::XmlSchemaContentProcessing::Lax;
    sequence->Items->Add(any1);
    ::System::Xml::Schema::XmlSchemaAny^  any2 = (gcnew ::System::Xml::Schema::XmlSchemaAny());
    any2->Namespace = L"urn:schemas-microsoft-com:xml-diffgram-v1";
    any2->MinOccurs = ::System::Decimal(1);
    any2->ProcessContents = ::System::Xml::Schema::XmlSchemaContentProcessing::Lax;
    sequence->Items->Add(any2);
    ::System::Xml::Schema::XmlSchemaAttribute^  attribute1 = (gcnew ::System::Xml::Schema::XmlSchemaAttribute());
    attribute1->Name = L"namespace";
    attribute1->FixedValue = ds->Namespace;
    type->Attributes->Add(attribute1);
    ::System::Xml::Schema::XmlSchemaAttribute^  attribute2 = (gcnew ::System::Xml::Schema::XmlSchemaAttribute());
    attribute2->Name = L"tableTypeName";
    attribute2->FixedValue = L"operationsDataTable";
    type->Attributes->Add(attribute2);
    type->Particle = sequence;
    ::System::Xml::Schema::XmlSchema^  dsSchema = ds->GetSchemaSerializable();
    if (xs->Contains(dsSchema->TargetNamespace)) {
        ::System::IO::MemoryStream^  s1 = (gcnew ::System::IO::MemoryStream());
        ::System::IO::MemoryStream^  s2 = (gcnew ::System::IO::MemoryStream());
        try {
            ::System::Xml::Schema::XmlSchema^  schema = nullptr;
            dsSchema->Write(s1);
            for (            ::System::Collections::IEnumerator^  schemas = xs->Schemas(dsSchema->TargetNamespace)->GetEnumerator(); schemas->MoveNext();             ) {
                schema = (cli::safe_cast<::System::Xml::Schema::XmlSchema^  >(schemas->Current));
                s2->SetLength(0);
                schema->Write(s2);
                if (s1->Length == s2->Length) {
                    s1->Position = 0;
                    s2->Position = 0;
                    for (                    ; ((s1->Position != s1->Length) 
                                && (s1->ReadByte() == s2->ReadByte()));                     ) {
                        ;
                    }
                    if (s1->Position == s1->Length) {
                        return type;
                    }
                }
            }
        }
        finally {
            if (s1 != nullptr) {
                s1->Close();
            }
            if (s2 != nullptr) {
                s2->Close();
            }
        }
    }
    xs->Add(dsSchema);
    return type;
}


inline NewDataSet::operationDataTable::operationDataTable() {
    this->TableName = L"operation";
    this->BeginInit();
    this->InitClass();
    this->EndInit();
}

inline NewDataSet::operationDataTable::operationDataTable(::System::Data::DataTable^  table) {
    this->TableName = table->TableName;
    if (table->CaseSensitive != table->DataSet->CaseSensitive) {
        this->CaseSensitive = table->CaseSensitive;
    }
    if (table->Locale->ToString() != table->DataSet->Locale->ToString()) {
        this->Locale = table->Locale;
    }
    if (table->Namespace != table->DataSet->Namespace) {
        this->Namespace = table->Namespace;
    }
    this->Prefix = table->Prefix;
    this->MinimumCapacity = table->MinimumCapacity;
}

inline NewDataSet::operationDataTable::operationDataTable(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context) : 
        ::System::Data::DataTable(info, context) {
    this->InitVars();
}

inline ::System::Data::DataColumn^  NewDataSet::operationDataTable::nameColumn::get() {
    return this->columnname;
}

inline ::System::Data::DataColumn^  NewDataSet::operationDataTable::_short_nameColumn::get() {
    return this->column_short_name;
}

inline ::System::Data::DataColumn^  NewDataSet::operationDataTable::descColumn::get() {
    return this->columndesc;
}

inline ::System::Data::DataColumn^  NewDataSet::operationDataTable::paramColumn::get() {
    return this->columnparam;
}

inline ::System::Data::DataColumn^  NewDataSet::operationDataTable::operation_IdColumn::get() {
    return this->columnoperation_Id;
}

inline ::System::Data::DataColumn^  NewDataSet::operationDataTable::operations_IdColumn::get() {
    return this->columnoperations_Id;
}

inline ::System::Int32 NewDataSet::operationDataTable::Count::get() {
    return this->Rows->Count;
}

inline NewDataSet::operationRow^  NewDataSet::operationDataTable::default::get(::System::Int32 index) {
    return (cli::safe_cast<NewDataSet::operationRow^  >(this->Rows[index]));
}

inline ::System::Void NewDataSet::operationDataTable::AddoperationRow(NewDataSet::operationRow^  row) {
    this->Rows->Add(row);
}

inline NewDataSet::operationRow^  NewDataSet::operationDataTable::AddoperationRow(System::String^  name, System::String^  _short_name, 
            System::String^  desc, System::String^  param, NewDataSet::operationsRow^  parentoperationsRowByoperations_operation) {
    NewDataSet::operationRow^  rowoperationRow = (cli::safe_cast<NewDataSet::operationRow^  >(this->NewRow()));
    cli::array< ::System::Object^  >^  columnValuesArray = gcnew cli::array< ::System::Object^  >(6) {name, _short_name, desc, 
        param, nullptr, nullptr};
    if (parentoperationsRowByoperations_operation != nullptr) {
        columnValuesArray[5] = parentoperationsRowByoperations_operation[2];
    }
    rowoperationRow->ItemArray = columnValuesArray;
    this->Rows->Add(rowoperationRow);
    return rowoperationRow;
}

inline ::System::Collections::IEnumerator^  NewDataSet::operationDataTable::GetEnumerator() {
    return this->Rows->GetEnumerator();
}

inline ::System::Data::DataTable^  NewDataSet::operationDataTable::Clone() {
    NewDataSet::operationDataTable^  cln = (cli::safe_cast<NewDataSet::operationDataTable^  >(__super::Clone()));
    cln->InitVars();
    return cln;
}

inline ::System::Data::DataTable^  NewDataSet::operationDataTable::CreateInstance() {
    return (gcnew NewDataSet::operationDataTable());
}

inline ::System::Void NewDataSet::operationDataTable::InitVars() {
    this->columnname = __super::Columns[L"name"];
    this->column_short_name = __super::Columns[L"short-name"];
    this->columndesc = __super::Columns[L"desc"];
    this->columnparam = __super::Columns[L"param"];
    this->columnoperation_Id = __super::Columns[L"operation_Id"];
    this->columnoperations_Id = __super::Columns[L"operations_Id"];
}

inline ::System::Void NewDataSet::operationDataTable::InitClass() {
    this->columnname = (gcnew ::System::Data::DataColumn(L"name", ::System::String::typeid, nullptr, ::System::Data::MappingType::Element));
    __super::Columns->Add(this->columnname);
    this->column_short_name = (gcnew ::System::Data::DataColumn(L"short-name", ::System::String::typeid, nullptr, ::System::Data::MappingType::Element));
    __super::Columns->Add(this->column_short_name);
    this->columndesc = (gcnew ::System::Data::DataColumn(L"desc", ::System::String::typeid, nullptr, ::System::Data::MappingType::Element));
    __super::Columns->Add(this->columndesc);
    this->columnparam = (gcnew ::System::Data::DataColumn(L"param", ::System::String::typeid, nullptr, ::System::Data::MappingType::Element));
    __super::Columns->Add(this->columnparam);
    this->columnoperation_Id = (gcnew ::System::Data::DataColumn(L"operation_Id", ::System::Int32::typeid, nullptr, ::System::Data::MappingType::Hidden));
    __super::Columns->Add(this->columnoperation_Id);
    this->columnoperations_Id = (gcnew ::System::Data::DataColumn(L"operations_Id", ::System::Int32::typeid, nullptr, ::System::Data::MappingType::Hidden));
    __super::Columns->Add(this->columnoperations_Id);
    this->Constraints->Add((gcnew ::System::Data::UniqueConstraint(L"Constraint1", gcnew cli::array< ::System::Data::DataColumn^  >(1) {this->columnoperation_Id}, 
            true)));
    this->columnname->AllowDBNull = false;
    this->column_short_name->AllowDBNull = false;
    this->columnparam->AllowDBNull = false;
    this->columnoperation_Id->AutoIncrement = true;
    this->columnoperation_Id->AllowDBNull = false;
    this->columnoperation_Id->Unique = true;
}

inline NewDataSet::operationRow^  NewDataSet::operationDataTable::NewoperationRow() {
    return (cli::safe_cast<NewDataSet::operationRow^  >(this->NewRow()));
}

inline ::System::Data::DataRow^  NewDataSet::operationDataTable::NewRowFromBuilder(::System::Data::DataRowBuilder^  builder) {
    return (gcnew NewDataSet::operationRow(builder));
}

inline ::System::Type^  NewDataSet::operationDataTable::GetRowType() {
    return NewDataSet::operationRow::typeid;
}

inline ::System::Void NewDataSet::operationDataTable::OnRowChanged(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowChanged(e);
    {
        this->operationRowChanged(this, (gcnew NewDataSet::operationRowChangeEvent((cli::safe_cast<NewDataSet::operationRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationDataTable::OnRowChanging(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowChanging(e);
    {
        this->operationRowChanging(this, (gcnew NewDataSet::operationRowChangeEvent((cli::safe_cast<NewDataSet::operationRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationDataTable::OnRowDeleted(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowDeleted(e);
    {
        this->operationRowDeleted(this, (gcnew NewDataSet::operationRowChangeEvent((cli::safe_cast<NewDataSet::operationRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationDataTable::OnRowDeleting(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowDeleting(e);
    {
        this->operationRowDeleting(this, (gcnew NewDataSet::operationRowChangeEvent((cli::safe_cast<NewDataSet::operationRow^  >(e->Row)), 
                e->Action)));
    }
}

inline ::System::Void NewDataSet::operationDataTable::RemoveoperationRow(NewDataSet::operationRow^  row) {
    this->Rows->Remove(row);
}

inline ::System::Xml::Schema::XmlSchemaComplexType^  NewDataSet::operationDataTable::GetTypedTableSchema(::System::Xml::Schema::XmlSchemaSet^  xs) {
    ::System::Xml::Schema::XmlSchemaComplexType^  type = (gcnew ::System::Xml::Schema::XmlSchemaComplexType());
    ::System::Xml::Schema::XmlSchemaSequence^  sequence = (gcnew ::System::Xml::Schema::XmlSchemaSequence());
    NewDataSet^  ds = (gcnew NewDataSet());
    ::System::Xml::Schema::XmlSchemaAny^  any1 = (gcnew ::System::Xml::Schema::XmlSchemaAny());
    any1->Namespace = L"http://www.w3.org/2001/XMLSchema";
    any1->MinOccurs = ::System::Decimal(0);
    any1->MaxOccurs = ::System::Decimal::MaxValue;
    any1->ProcessContents = ::System::Xml::Schema::XmlSchemaContentProcessing::Lax;
    sequence->Items->Add(any1);
    ::System::Xml::Schema::XmlSchemaAny^  any2 = (gcnew ::System::Xml::Schema::XmlSchemaAny());
    any2->Namespace = L"urn:schemas-microsoft-com:xml-diffgram-v1";
    any2->MinOccurs = ::System::Decimal(1);
    any2->ProcessContents = ::System::Xml::Schema::XmlSchemaContentProcessing::Lax;
    sequence->Items->Add(any2);
    ::System::Xml::Schema::XmlSchemaAttribute^  attribute1 = (gcnew ::System::Xml::Schema::XmlSchemaAttribute());
    attribute1->Name = L"namespace";
    attribute1->FixedValue = ds->Namespace;
    type->Attributes->Add(attribute1);
    ::System::Xml::Schema::XmlSchemaAttribute^  attribute2 = (gcnew ::System::Xml::Schema::XmlSchemaAttribute());
    attribute2->Name = L"tableTypeName";
    attribute2->FixedValue = L"operationDataTable";
    type->Attributes->Add(attribute2);
    type->Particle = sequence;
    ::System::Xml::Schema::XmlSchema^  dsSchema = ds->GetSchemaSerializable();
    if (xs->Contains(dsSchema->TargetNamespace)) {
        ::System::IO::MemoryStream^  s1 = (gcnew ::System::IO::MemoryStream());
        ::System::IO::MemoryStream^  s2 = (gcnew ::System::IO::MemoryStream());
        try {
            ::System::Xml::Schema::XmlSchema^  schema = nullptr;
            dsSchema->Write(s1);
            for (            ::System::Collections::IEnumerator^  schemas = xs->Schemas(dsSchema->TargetNamespace)->GetEnumerator(); schemas->MoveNext();             ) {
                schema = (cli::safe_cast<::System::Xml::Schema::XmlSchema^  >(schemas->Current));
                s2->SetLength(0);
                schema->Write(s2);
                if (s1->Length == s2->Length) {
                    s1->Position = 0;
                    s2->Position = 0;
                    for (                    ; ((s1->Position != s1->Length) 
                                && (s1->ReadByte() == s2->ReadByte()));                     ) {
                        ;
                    }
                    if (s1->Position == s1->Length) {
                        return type;
                    }
                }
            }
        }
        finally {
            if (s1 != nullptr) {
                s1->Close();
            }
            if (s2 != nullptr) {
                s2->Close();
            }
        }
    }
    xs->Add(dsSchema);
    return type;
}


inline NewDataSet::dataDataTable::dataDataTable() {
    this->TableName = L"data";
    this->BeginInit();
    this->InitClass();
    this->EndInit();
}

inline NewDataSet::dataDataTable::dataDataTable(::System::Data::DataTable^  table) {
    this->TableName = table->TableName;
    if (table->CaseSensitive != table->DataSet->CaseSensitive) {
        this->CaseSensitive = table->CaseSensitive;
    }
    if (table->Locale->ToString() != table->DataSet->Locale->ToString()) {
        this->Locale = table->Locale;
    }
    if (table->Namespace != table->DataSet->Namespace) {
        this->Namespace = table->Namespace;
    }
    this->Prefix = table->Prefix;
    this->MinimumCapacity = table->MinimumCapacity;
}

inline NewDataSet::dataDataTable::dataDataTable(::System::Runtime::Serialization::SerializationInfo^  info, ::System::Runtime::Serialization::StreamingContext context) : 
        ::System::Data::DataTable(info, context) {
    this->InitVars();
}

inline ::System::Data::DataColumn^  NewDataSet::dataDataTable::beforeColumn::get() {
    return this->columnbefore;
}

inline ::System::Data::DataColumn^  NewDataSet::dataDataTable::afterColumn::get() {
    return this->columnafter;
}

inline ::System::Data::DataColumn^  NewDataSet::dataDataTable::operation_IdColumn::get() {
    return this->columnoperation_Id;
}

inline ::System::Int32 NewDataSet::dataDataTable::Count::get() {
    return this->Rows->Count;
}

inline NewDataSet::dataRow^  NewDataSet::dataDataTable::default::get(::System::Int32 index) {
    return (cli::safe_cast<NewDataSet::dataRow^  >(this->Rows[index]));
}

inline ::System::Void NewDataSet::dataDataTable::AdddataRow(NewDataSet::dataRow^  row) {
    this->Rows->Add(row);
}

inline NewDataSet::dataRow^  NewDataSet::dataDataTable::AdddataRow(cli::array< System::Byte >^  before, cli::array< System::Byte >^  after, 
            NewDataSet::operationRow^  parentoperationRowByoperation_data) {
    NewDataSet::dataRow^  rowdataRow = (cli::safe_cast<NewDataSet::dataRow^  >(this->NewRow()));
    cli::array< ::System::Object^  >^  columnValuesArray = gcnew cli::array< ::System::Object^  >(3) {before, after, nullptr};
    if (parentoperationRowByoperation_data != nullptr) {
        columnValuesArray[2] = parentoperationRowByoperation_data[4];
    }
    rowdataRow->ItemArray = columnValuesArray;
    this->Rows->Add(rowdataRow);
    return rowdataRow;
}

inline ::System::Collections::IEnumerator^  NewDataSet::dataDataTable::GetEnumerator() {
    return this->Rows->GetEnumerator();
}

inline ::System::Data::DataTable^  NewDataSet::dataDataTable::Clone() {
    NewDataSet::dataDataTable^  cln = (cli::safe_cast<NewDataSet::dataDataTable^  >(__super::Clone()));
    cln->InitVars();
    return cln;
}

inline ::System::Data::DataTable^  NewDataSet::dataDataTable::CreateInstance() {
    return (gcnew NewDataSet::dataDataTable());
}

inline ::System::Void NewDataSet::dataDataTable::InitVars() {
    this->columnbefore = __super::Columns[L"before"];
    this->columnafter = __super::Columns[L"after"];
    this->columnoperation_Id = __super::Columns[L"operation_Id"];
}

inline ::System::Void NewDataSet::dataDataTable::InitClass() {
    this->columnbefore = (gcnew ::System::Data::DataColumn(L"before", cli::array< ::System::Byte >::typeid, nullptr, ::System::Data::MappingType::Element));
    __super::Columns->Add(this->columnbefore);
    this->columnafter = (gcnew ::System::Data::DataColumn(L"after", cli::array< ::System::Byte >::typeid, nullptr, ::System::Data::MappingType::Element));
    __super::Columns->Add(this->columnafter);
    this->columnoperation_Id = (gcnew ::System::Data::DataColumn(L"operation_Id", ::System::Int32::typeid, nullptr, ::System::Data::MappingType::Hidden));
    __super::Columns->Add(this->columnoperation_Id);
    this->columnbefore->AllowDBNull = false;
    this->columnafter->AllowDBNull = false;
}

inline NewDataSet::dataRow^  NewDataSet::dataDataTable::NewdataRow() {
    return (cli::safe_cast<NewDataSet::dataRow^  >(this->NewRow()));
}

inline ::System::Data::DataRow^  NewDataSet::dataDataTable::NewRowFromBuilder(::System::Data::DataRowBuilder^  builder) {
    return (gcnew NewDataSet::dataRow(builder));
}

inline ::System::Type^  NewDataSet::dataDataTable::GetRowType() {
    return NewDataSet::dataRow::typeid;
}

inline ::System::Void NewDataSet::dataDataTable::OnRowChanged(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowChanged(e);
    {
        this->dataRowChanged(this, (gcnew NewDataSet::dataRowChangeEvent((cli::safe_cast<NewDataSet::dataRow^  >(e->Row)), e->Action)));
    }
}

inline ::System::Void NewDataSet::dataDataTable::OnRowChanging(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowChanging(e);
    {
        this->dataRowChanging(this, (gcnew NewDataSet::dataRowChangeEvent((cli::safe_cast<NewDataSet::dataRow^  >(e->Row)), e->Action)));
    }
}

inline ::System::Void NewDataSet::dataDataTable::OnRowDeleted(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowDeleted(e);
    {
        this->dataRowDeleted(this, (gcnew NewDataSet::dataRowChangeEvent((cli::safe_cast<NewDataSet::dataRow^  >(e->Row)), e->Action)));
    }
}

inline ::System::Void NewDataSet::dataDataTable::OnRowDeleting(::System::Data::DataRowChangeEventArgs^  e) {
    __super::OnRowDeleting(e);
    {
        this->dataRowDeleting(this, (gcnew NewDataSet::dataRowChangeEvent((cli::safe_cast<NewDataSet::dataRow^  >(e->Row)), e->Action)));
    }
}

inline ::System::Void NewDataSet::dataDataTable::RemovedataRow(NewDataSet::dataRow^  row) {
    this->Rows->Remove(row);
}

inline ::System::Xml::Schema::XmlSchemaComplexType^  NewDataSet::dataDataTable::GetTypedTableSchema(::System::Xml::Schema::XmlSchemaSet^  xs) {
    ::System::Xml::Schema::XmlSchemaComplexType^  type = (gcnew ::System::Xml::Schema::XmlSchemaComplexType());
    ::System::Xml::Schema::XmlSchemaSequence^  sequence = (gcnew ::System::Xml::Schema::XmlSchemaSequence());
    NewDataSet^  ds = (gcnew NewDataSet());
    ::System::Xml::Schema::XmlSchemaAny^  any1 = (gcnew ::System::Xml::Schema::XmlSchemaAny());
    any1->Namespace = L"http://www.w3.org/2001/XMLSchema";
    any1->MinOccurs = ::System::Decimal(0);
    any1->MaxOccurs = ::System::Decimal::MaxValue;
    any1->ProcessContents = ::System::Xml::Schema::XmlSchemaContentProcessing::Lax;
    sequence->Items->Add(any1);
    ::System::Xml::Schema::XmlSchemaAny^  any2 = (gcnew ::System::Xml::Schema::XmlSchemaAny());
    any2->Namespace = L"urn:schemas-microsoft-com:xml-diffgram-v1";
    any2->MinOccurs = ::System::Decimal(1);
    any2->ProcessContents = ::System::Xml::Schema::XmlSchemaContentProcessing::Lax;
    sequence->Items->Add(any2);
    ::System::Xml::Schema::XmlSchemaAttribute^  attribute1 = (gcnew ::System::Xml::Schema::XmlSchemaAttribute());
    attribute1->Name = L"namespace";
    attribute1->FixedValue = ds->Namespace;
    type->Attributes->Add(attribute1);
    ::System::Xml::Schema::XmlSchemaAttribute^  attribute2 = (gcnew ::System::Xml::Schema::XmlSchemaAttribute());
    attribute2->Name = L"tableTypeName";
    attribute2->FixedValue = L"dataDataTable";
    type->Attributes->Add(attribute2);
    type->Particle = sequence;
    ::System::Xml::Schema::XmlSchema^  dsSchema = ds->GetSchemaSerializable();
    if (xs->Contains(dsSchema->TargetNamespace)) {
        ::System::IO::MemoryStream^  s1 = (gcnew ::System::IO::MemoryStream());
        ::System::IO::MemoryStream^  s2 = (gcnew ::System::IO::MemoryStream());
        try {
            ::System::Xml::Schema::XmlSchema^  schema = nullptr;
            dsSchema->Write(s1);
            for (            ::System::Collections::IEnumerator^  schemas = xs->Schemas(dsSchema->TargetNamespace)->GetEnumerator(); schemas->MoveNext();             ) {
                schema = (cli::safe_cast<::System::Xml::Schema::XmlSchema^  >(schemas->Current));
                s2->SetLength(0);
                schema->Write(s2);
                if (s1->Length == s2->Length) {
                    s1->Position = 0;
                    s2->Position = 0;
                    for (                    ; ((s1->Position != s1->Length) 
                                && (s1->ReadByte() == s2->ReadByte()));                     ) {
                        ;
                    }
                    if (s1->Position == s1->Length) {
                        return type;
                    }
                }
            }
        }
        finally {
            if (s1 != nullptr) {
                s1->Close();
            }
            if (s2 != nullptr) {
                s2->Close();
            }
        }
    }
    xs->Add(dsSchema);
    return type;
}


inline NewDataSet::operationsRow::operationsRow(::System::Data::DataRowBuilder^  rb) : 
        ::System::Data::DataRow(rb) {
    this->tableoperations = (cli::safe_cast<NewDataSet::operationsDataTable^  >(this->Table));
}

inline System::String^  NewDataSet::operationsRow::type::get() {
    try {
        return (cli::safe_cast<::System::String^  >(this[this->tableoperations->typeColumn]));
    }
    catch (::System::InvalidCastException^ e) {
        throw (gcnew ::System::Data::StrongTypingException(L"テーブル \'operations\' にある列 \'type\' の値は DBNull です。", e));
    }
}
inline System::Void NewDataSet::operationsRow::type::set(System::String^  value) {
    this[this->tableoperations->typeColumn] = value;
}

inline System::Int64 NewDataSet::operationsRow::state::get() {
    try {
        return (cli::safe_cast<::System::Int64 >(this[this->tableoperations->stateColumn]));
    }
    catch (::System::InvalidCastException^ e) {
        throw (gcnew ::System::Data::StrongTypingException(L"テーブル \'operations\' にある列 \'state\' の値は DBNull です。", e));
    }
}
inline System::Void NewDataSet::operationsRow::state::set(System::Int64 value) {
    this[this->tableoperations->stateColumn] = value;
}

inline System::Int32 NewDataSet::operationsRow::operations_Id::get() {
    return (cli::safe_cast<::System::Int32 >(this[this->tableoperations->operations_IdColumn]));
}
inline System::Void NewDataSet::operationsRow::operations_Id::set(System::Int32 value) {
    this[this->tableoperations->operations_IdColumn] = value;
}

inline ::System::Boolean NewDataSet::operationsRow::IstypeNull() {
    return this->IsNull(this->tableoperations->typeColumn);
}

inline ::System::Void NewDataSet::operationsRow::SettypeNull() {
    this[this->tableoperations->typeColumn] = ::System::Convert::DBNull;
}

inline ::System::Boolean NewDataSet::operationsRow::IsstateNull() {
    return this->IsNull(this->tableoperations->stateColumn);
}

inline ::System::Void NewDataSet::operationsRow::SetstateNull() {
    this[this->tableoperations->stateColumn] = ::System::Convert::DBNull;
}

inline cli::array< NewDataSet::operationRow^  >^  NewDataSet::operationsRow::GetoperationRows() {
    if (this->Table->ChildRelations[L"operations_operation"] == nullptr) {
        return gcnew cli::array< NewDataSet::operationRow^  >(0);
    }
    else {
        return (cli::safe_cast<cli::array< NewDataSet::operationRow^  >^  >(__super::GetChildRows(this->Table->ChildRelations[L"operations_operation"])));
    }
}


inline NewDataSet::operationRow::operationRow(::System::Data::DataRowBuilder^  rb) : 
        ::System::Data::DataRow(rb) {
    this->tableoperation = (cli::safe_cast<NewDataSet::operationDataTable^  >(this->Table));
}

inline System::String^  NewDataSet::operationRow::name::get() {
    return (cli::safe_cast<::System::String^  >(this[this->tableoperation->nameColumn]));
}
inline System::Void NewDataSet::operationRow::name::set(System::String^  value) {
    this[this->tableoperation->nameColumn] = value;
}

inline System::String^  NewDataSet::operationRow::_short_name::get() {
    return (cli::safe_cast<::System::String^  >(this[this->tableoperation->_short_nameColumn]));
}
inline System::Void NewDataSet::operationRow::_short_name::set(System::String^  value) {
    this[this->tableoperation->_short_nameColumn] = value;
}

inline System::String^  NewDataSet::operationRow::desc::get() {
    try {
        return (cli::safe_cast<::System::String^  >(this[this->tableoperation->descColumn]));
    }
    catch (::System::InvalidCastException^ e) {
        throw (gcnew ::System::Data::StrongTypingException(L"テーブル \'operation\' にある列 \'desc\' の値は DBNull です。", e));
    }
}
inline System::Void NewDataSet::operationRow::desc::set(System::String^  value) {
    this[this->tableoperation->descColumn] = value;
}

inline System::String^  NewDataSet::operationRow::param::get() {
    return (cli::safe_cast<::System::String^  >(this[this->tableoperation->paramColumn]));
}
inline System::Void NewDataSet::operationRow::param::set(System::String^  value) {
    this[this->tableoperation->paramColumn] = value;
}

inline System::Int32 NewDataSet::operationRow::operation_Id::get() {
    return (cli::safe_cast<::System::Int32 >(this[this->tableoperation->operation_IdColumn]));
}
inline System::Void NewDataSet::operationRow::operation_Id::set(System::Int32 value) {
    this[this->tableoperation->operation_IdColumn] = value;
}

inline System::Int32 NewDataSet::operationRow::operations_Id::get() {
    try {
        return (cli::safe_cast<::System::Int32 >(this[this->tableoperation->operations_IdColumn]));
    }
    catch (::System::InvalidCastException^ e) {
        throw (gcnew ::System::Data::StrongTypingException(L"テーブル \'operation\' にある列 \'operations_Id\' の値は DBNull です。", e));
    }
}
inline System::Void NewDataSet::operationRow::operations_Id::set(System::Int32 value) {
    this[this->tableoperation->operations_IdColumn] = value;
}

inline NewDataSet::operationsRow^  NewDataSet::operationRow::operationsRow::get() {
    return (cli::safe_cast<NewDataSet::operationsRow^  >(this->GetParentRow(this->Table->ParentRelations[L"operations_operation"])));
}
inline System::Void NewDataSet::operationRow::operationsRow::set(NewDataSet::operationsRow^  value) {
    this->SetParentRow(value, this->Table->ParentRelations[L"operations_operation"]);
}

inline ::System::Boolean NewDataSet::operationRow::IsdescNull() {
    return this->IsNull(this->tableoperation->descColumn);
}

inline ::System::Void NewDataSet::operationRow::SetdescNull() {
    this[this->tableoperation->descColumn] = ::System::Convert::DBNull;
}

inline ::System::Boolean NewDataSet::operationRow::Isoperations_IdNull() {
    return this->IsNull(this->tableoperation->operations_IdColumn);
}

inline ::System::Void NewDataSet::operationRow::Setoperations_IdNull() {
    this[this->tableoperation->operations_IdColumn] = ::System::Convert::DBNull;
}

inline cli::array< NewDataSet::dataRow^  >^  NewDataSet::operationRow::GetdataRows() {
    if (this->Table->ChildRelations[L"operation_data"] == nullptr) {
        return gcnew cli::array< NewDataSet::dataRow^  >(0);
    }
    else {
        return (cli::safe_cast<cli::array< NewDataSet::dataRow^  >^  >(__super::GetChildRows(this->Table->ChildRelations[L"operation_data"])));
    }
}


inline NewDataSet::dataRow::dataRow(::System::Data::DataRowBuilder^  rb) : 
        ::System::Data::DataRow(rb) {
    this->tabledata = (cli::safe_cast<NewDataSet::dataDataTable^  >(this->Table));
}

inline cli::array< System::Byte >^  NewDataSet::dataRow::before::get() {
    return (cli::safe_cast<cli::array< ::System::Byte >^  >(this[this->tabledata->beforeColumn]));
}
inline System::Void NewDataSet::dataRow::before::set(cli::array< System::Byte >^  value) {
    this[this->tabledata->beforeColumn] = value;
}

inline cli::array< System::Byte >^  NewDataSet::dataRow::after::get() {
    return (cli::safe_cast<cli::array< ::System::Byte >^  >(this[this->tabledata->afterColumn]));
}
inline System::Void NewDataSet::dataRow::after::set(cli::array< System::Byte >^  value) {
    this[this->tabledata->afterColumn] = value;
}

inline System::Int32 NewDataSet::dataRow::operation_Id::get() {
    try {
        return (cli::safe_cast<::System::Int32 >(this[this->tabledata->operation_IdColumn]));
    }
    catch (::System::InvalidCastException^ e) {
        throw (gcnew ::System::Data::StrongTypingException(L"テーブル \'data\' にある列 \'operation_Id\' の値は DBNull です。", e));
    }
}
inline System::Void NewDataSet::dataRow::operation_Id::set(System::Int32 value) {
    this[this->tabledata->operation_IdColumn] = value;
}

inline NewDataSet::operationRow^  NewDataSet::dataRow::operationRow::get() {
    return (cli::safe_cast<NewDataSet::operationRow^  >(this->GetParentRow(this->Table->ParentRelations[L"operation_data"])));
}
inline System::Void NewDataSet::dataRow::operationRow::set(NewDataSet::operationRow^  value) {
    this->SetParentRow(value, this->Table->ParentRelations[L"operation_data"]);
}

inline ::System::Boolean NewDataSet::dataRow::Isoperation_IdNull() {
    return this->IsNull(this->tabledata->operation_IdColumn);
}

inline ::System::Void NewDataSet::dataRow::Setoperation_IdNull() {
    this[this->tabledata->operation_IdColumn] = ::System::Convert::DBNull;
}


inline NewDataSet::operationsRowChangeEvent::operationsRowChangeEvent(NewDataSet::operationsRow^  row, ::System::Data::DataRowAction action) {
    this->eventRow = row;
    this->eventAction = action;
}

inline NewDataSet::operationsRow^  NewDataSet::operationsRowChangeEvent::Row::get() {
    return this->eventRow;
}

inline ::System::Data::DataRowAction NewDataSet::operationsRowChangeEvent::Action::get() {
    return this->eventAction;
}


inline NewDataSet::operationRowChangeEvent::operationRowChangeEvent(NewDataSet::operationRow^  row, ::System::Data::DataRowAction action) {
    this->eventRow = row;
    this->eventAction = action;
}

inline NewDataSet::operationRow^  NewDataSet::operationRowChangeEvent::Row::get() {
    return this->eventRow;
}

inline ::System::Data::DataRowAction NewDataSet::operationRowChangeEvent::Action::get() {
    return this->eventAction;
}


inline NewDataSet::dataRowChangeEvent::dataRowChangeEvent(NewDataSet::dataRow^  row, ::System::Data::DataRowAction action) {
    this->eventRow = row;
    this->eventAction = action;
}

inline NewDataSet::dataRow^  NewDataSet::dataRowChangeEvent::Row::get() {
    return this->eventRow;
}

inline ::System::Data::DataRowAction NewDataSet::dataRowChangeEvent::Action::get() {
    return this->eventAction;
}
