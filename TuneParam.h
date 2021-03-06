//#pragma once
#ifndef TUNE_PARAM_H_
#define TUNE_PARAM_H_
//������ ������������ ��� �������� ������ ����������� � ������ ��������
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "TConstants.h"
#include "libxl.h"

//�������� ������������ ���� ��� ������ � �����������
namespace NS_Tune
{
	using std::string;
	using std::ifstream;
	using std::pair;
	using std::vector;
	using std::set;
	using std::size_t;
	using find_fnc = size_t (string::*)(const string& s, size_t pos) const;
	using TColor = libxl::Color;
	using NS_Const::TuneField;
	using NS_Const::DataType;
	using NS_Const::CtrlSym;
	using NS_Const::TConstField;
	using NS_Const::TConstType;
	using NS_Const::TConstCtrlSym;
	using NS_Const::JsonParams;
	using NS_Const::JSonMeth;
	using NS_Const::TConstJSMeth;
	using boost::property_tree::ptree;


	class TBaseParam
	{
	private:
		string src_data;//�������� ������
	protected:
		string value;//�������� ���������
		size_t get_pos_in_src(const string& substr, const size_t beg = 0, find_fnc ff = &string::find) const;
		size_t get_pos_in_src(const TConstField& substr, const size_t beg = 0, find_fnc ff = &string::find) const;
		size_t get_pos_in_src(const CtrlSym& substr, const size_t beg = 0, find_fnc ff = &string::find) const;
		//��������� �������� �� ������ ������:
		virtual string Get_Str_Val(size_t pos, const CtrlSym& b_delimeter, const CtrlSym& e_delimeter,
			bool from_end = false) const;
		//�� ��������� ��������� ���������������� ����� � ������:
		virtual string Get_TuneField_Val(const TConstField& param, const CtrlSym& b_delimeter,
			const CtrlSym& e_delimeter, bool pose_from_end = false) const;
		void setSrcData(const string& val) { src_data = val; };
		virtual void setValue(const TConstField&, const CtrlSym& open_val, const CtrlSym& close_val) = 0;
	public:
		explicit TBaseParam(const string& full_str) : src_data(full_str) {}
		TBaseParam(const TBaseParam& x) : src_data(x.src_data), value(x.value) {}
		virtual bool isEmpty() const { return src_data.empty(); }
		virtual string toStr() const { return src_data; }
		virtual string Value() const { return value; }
		size_t srcSize() const { return src_data.size(); }
		string srcSubStr(size_t posb, size_t pose) const { return src_data.substr(posb, pose - posb); }
		size_t srcFind(const string& substr, size_t pos) const { return src_data.find(substr, pos); }
		//������ ��������� ��������
		virtual void Value(const string& val) { value = val; }
	};

	//����� ���������� �� ������:
	class TStringParam: public TBaseParam
	{
	private:
		TConstField param;//��������
	protected:
		virtual void setValue(const TConstField&, const CtrlSym& open_val, const CtrlSym& close_val);
	public:
		TStringParam() : TBaseParam(string()), param(TuneField::Empty) {};
		explicit TStringParam(const TConstField& val) : TBaseParam(string()), param(val) {}
		//������ ������������� ����������
		TStringParam(const TuneField& tune_field, const string& str, bool parse_val = true);
		//������������� ������� � ����������� �������� ���������
		TStringParam(const string& full_data, const TuneField& tune_field,
			const CtrlSym& open_val = CtrlSym::quotes, const CtrlSym& close_val = CtrlSym::quotes);
		TStringParam(const TStringParam& val): TBaseParam(val), param(val.param) {}
		virtual bool isEmpty() const { return (param.isEmpty() or value.empty()); }
		virtual string ParamName() const { return param.toStr(); }
		TuneField Param() const { return param.Value(); }
		bool operator<(const TuneField& x) const { return param < x; }
		bool operator>(const TuneField& x) const { return param > x; }
		bool operator==(const TuneField& x) const { return param == x; }
		string toStr(bool use_quotes = false) const;
		bool operator<(const TStringParam& x) const { return param < x.param; }
		bool operator==(const TStringParam& x) const { return operator==(x.Param()); }
		//������������ ���� ��� ���������� �������
		//virtual pair<TuneField, TStringParam> toPair() const { return std::make_pair(param.Value(), *this); };
	};

	//����� ��� ��������� ����������:
	class TSubParam: public TBaseParam
	{
	private:
		const int EmptyID = 0;
		int id;//����� ���������
		TConstType type;//��� ���������
		string comment;//����������
	protected:
		virtual void setValue(void);
		virtual void setValue(const TConstField&, const CtrlSym&, const CtrlSym&);
	public:
		explicit TSubParam(int val) : TBaseParam(string()), id(val), type(DataType::ErrorType), comment() {}
		explicit TSubParam(const string& str);
		TSubParam(const TSubParam& x) : TBaseParam(x), id(x.id), type(x.type), comment(x.comment) {}
		//��������� ID ���������
		int ID() const { return id; }
		//��������� ���� ������ � ���� ������
		string Type() const { return type.toStr(); }
		//��������� ���� ������ � ���� �������
		DataType DataType() const { return type.Value(); }
		//��������� ����������
		string Comment() const { return comment; }
		//��������� ���� ���������:
		string getCode() const;
		//��������� �������� �������������:
		bool setValByUser();
		//�������� �������:
		void show(std::ostream& stream = std::cout) const;
		//�������� < ��� ����������:
		bool operator<(const TSubParam& val) const { return id < val.id; }
		bool operator<(const int& x) const { return id < x; }
		bool operator>(const int& x) const { return id > x; }
		bool operator==(const int& x) const { return id == x; }
	};

	using TSubParamType = int;

	using TField = std::pair<TuneField, TStringParam>;
	using TParam = std::pair<int, TSubParam>;
	using StrArr = std::vector<string>;
	using FileParam = std::pair<string, string>;
	using TFields = std::vector<TStringParam>;
	using ParamArr = std::vector<TSubParam>;
	using TuneRange = std::pair<TuneField, TuneField>;
	using RangeField = std::set<TConstField>;

	//��������� ������������:
	//������� �����
	class TSimpleTune
	{
	protected:
		TFields fields;//���� �������� � �� ����������
		enum class Types { Config, SQL, DQL, DML, Template, OutPath, OutName, OutSheet };
		enum class TRead { Section, TuneVal };
	private:
		TSimpleTune& operator=(const TSimpleTune& v);
	protected:
		//������� ���������� ����������� � �����:
		static void AddDelimeter(string& str, const char delim) noexcept(false);
		//������� ��������� ����/���������� �� ��������:
		string getPathByCode(const Types& code) const noexcept(true);
		//������� ��������� ���� �� ����:
		FileParam getFileParamByCode(const Types& code) const noexcept(true);
		//������� ��������� ������ ������ � �������� �� ��������:
		vector<string> getFileLst(const Types& code, bool use_sort = true) const noexcept(false);
		//��������� ������� ��� ������ ���� � ������:
		//�������� �� ���������� boost\date_time ��� ������������� date_facet
		//������������ �������� ������ 
		//https://www.boost.org/doc/libs/1_49_0/doc/html/date_time/date_time_io.html
		static bool set_date_format(std::ostream& stream, const string& fromat) noexcept(true);
		static string cur_date_to_string_by_format(const string& format) noexcept(false);
		//������� ��������� ����� �� ���� ��������(������������� ������� ����)
		string AddCurDate2Name(const Types& code) const noexcept(false);
		//������� ���������� ���� ���������:
		bool AddField(const TStringParam& val) noexcept(true);
		//������� ��������� �������� ���������:
		bool setFieldVal(const TuneField& key, const string& val) noexcept(false);
		//������� ��������� ����������� �� ��������� ������ ������� ����������:
		virtual TuneRange getTuneRange(const TRead& x) const noexcept(true) = 0;
		//������� ��������� ������ ���������� � ��������� ���������
		static RangeField getTuneField4Range(const TuneRange& range);
		//������� ������ - ���������� ����� �� ������� [END]:
		static void skip_block(ifstream& file, const string& end_block);
		//������� ���������� ������ �������� �� ����� �� ����
		virtual void Read_Section(ifstream& file, const string& code) = 0;
		//������� ������ �������� �������� �� �����:
		virtual void Read_Tune_Val(ifstream& file);
		//������ �������� �� �����:
		virtual void ReadFromFile(const string& file);
		//template <typename KeyType, typename ValType>
		//ValType& getValueByID(const KeyType& par_ID, set<ValType>& arr) noexcept(false);
		//������ ������� ��� ��������� �������� ���� �� ��� ����������� ����/ID
		template <typename KeyType, typename ValType>
		static ValType& getElementByID(const KeyType& par_ID, vector<ValType>& arr)  noexcept(false);
		template <typename KeyType, typename ValType>
		static const ValType& getConstElementByID(const KeyType& par_ID, const vector<ValType>& arr)  noexcept(false);
		/*
		template <typename KeyType, typename ValType>
		static const ValType& NS_Tune::TSimpleTune::getValueByID(const KeyType& par_ID, const set<ValType>& arr) noexcept(false);
		/**/
		//������� ��������� �������� ��� ���������:
		//��������� ����� ���������� �����/��������/����:
		string getNameByCode(const Types& code) const noexcept(true);
		//��������� ������� ����� ����� �� ����:
		string getFullFileName(const Types& code, bool only_path = false) const noexcept(true);
	public:
		TSimpleTune() {}
		//������������� ������ ����������
		explicit TSimpleTune(const TSimpleTune& v) : fields(v.fields) {}
		explicit TSimpleTune(const TFields& v) : fields(v) {}
		~TSimpleTune() {}
		//�������� �� ������� ��� ����������� ������:
		virtual bool Empty() const { return fields.empty(); }
		//��������� ��������:
		string getFieldValueByCode(const TuneField& code) const noexcept(true);
		//��������� �������������� �������� �� ���� ���������:
		//���� ������� false - ��������� ������ ��� ��������� ��������
		bool FieldValueAsInt(const TuneField& code, int& val) const noexcept(false);
		//������� ����������� ������ ��������:
		virtual void show_tunes(std::ostream& stream = std::cout) const;
		//���� ������� ��������� �� ������� ����� ������������ �������� �� ���������
		string getOutSheet() const noexcept(false) { return getFullFileName(Types::OutSheet); }
		//������� ��������� ����� ��������� ����� �� ��������� ��������:
		string getOutFileBySheet() const noexcept(false);
		//������� �������� �������� bool-���������:
		bool useFlag(const TuneField& code) const noexcept(true);
		//������� ��������� ������ ������ � ��������:
		static vector<string> getFileLst(const string& file_dir, const string& file_ext = "", bool use_sort = true) noexcept(false);
	};
	
	//����� ��������� ������������:
	class TSharedTune : public TSimpleTune
	{
	private:
		string  main_code;//��� �������� ���������
		TSharedTune& operator=(const TSharedTune& v);
		//��������� ����� ������ �������������� �������� ������
		string getSectionName() const noexcept(true);
		virtual TuneRange getTuneRange(const TRead& x) const noexcept(true);
		//���������� ������
		virtual void Read_Section(ifstream& file, const string& code);
	public:
		//�������������
		TSharedTune(const string& file, const string& code);
		TSharedTune(const TFields& v, const string& file, const string& code);
		TSharedTune(const TSharedTune& v, const string& file, const string& code);
		~TSharedTune() {}
		//��� �������� ���������
		string getMainCode(bool use_brkt = true) const;
		//��������� �������� �������� ���������:
		string getMainPathVal() const { return getFieldValueByCode(TuneField::MainPath); }
		//�������� �������:
		bool Empty() const { return TSimpleTune::Empty(); }
		//��������� ������ ������:
		vector<string> getConfFileLst(bool use_sort = true) const noexcept(false) { return getFileLst(Types::Config, use_sort); }
		vector<string> getSqlFileLst(bool use_sort = true) const noexcept(false) { return getFileLst(Types::DQL, use_sort); }
		vector<string> getTemplFileLst(bool use_sort = true) const noexcept(false) { return getFileLst(Types::Template, use_sort); }
		//��������� ���� � ����� ��������:
		string getConfigPath() const noexcept(false) { return getFullFileName(Types::Config, true); }
		//��������� ����� ���������� �����:
		string getOutPath() const noexcept(false) { return getFullFileName(Types::OutPath, true); }
		string getOutFile() const noexcept(false) { return getFullFileName(Types::OutName, false); }
		//�������� ������������� ���������� - ���� �� ���������� - �������:
		static bool CheckPath(const string& dir, bool crt_if_not_found = true);
	};

	//��������� ������������ ��� �������� ������
	class TUserTune: public TSimpleTune
	{
	private:
		StrArr cols;
		ParamArr params;
		virtual TuneRange getTuneRange(const TRead& x) const noexcept(true);
		//������� ���������� ������ �������� �� ����� �� ����
		void Read_Section(ifstream& file, const string& code);
		//������� ���������� ��������� ����������:
		void Read_StreamData_Val(ifstream& file, const TuneField& stream_title, StrArr& str_arr);
		//������� ���������� ������ � ���������� �������:
		void Read_Param_Val(ifstream& file);
		//������� ������ �������� �������
		void Read_Col_Val(ifstream& file);
		TUserTune& operator=(const TUserTune& v);
		//��������� ������� sql-������ ��� ����������:
		vector<string> getSQLFileLst(const Types& kind, bool use_sort = true) const noexcept(false);
		bool isEmptyByCode(const Types& kind) const noexcept(false);
	public:
		//������ �������� ����������� �� �����
		explicit TUserTune(const string& tunefile);
		//������������� ���������� �������� � ����� ������
		explicit TUserTune(const TSimpleTune& tune, const string& file);
		TUserTune(const TUserTune& tune) : TSimpleTune(tune), cols(tune.cols), params(tune.params) {}
		~TUserTune() {};
		//�������� �� ������� ��� �����:
		virtual bool EmptyFields(void) const { return TSimpleTune::Empty(); }
		//�������� ������ �� ������ �������
		bool EmptyColumns(void) const { return cols.empty(); }
		//�������� �� ������� ����������:
		bool EmptyParams(void) const { return params.empty(); }
		//�������� �� ������� ��� ����������� ������:
		bool Empty() const { return EmptyFields() && EmptyColumns(); }
		//��������� �������� ��� ��������� �� ID:
		string getParamValByID(int par_id, bool exit_on_er = true) const noexcept(false);
		//��������� ���������:
		TSubParam getParamByID(int par_id, bool exit_on_er = true) const noexcept(false);
		//��������� ������� ����������:
		ParamArr getParams() const { return params; }
		//������� ��������� �������� ��� ������� � ��������� ��������:
		string getColValByIndx(int indx) const { return cols[indx]; }
		//��������� ������ �������:
		StrArr getColumns(void) const { return cols; }
		//������� ����������� ������ �������:
		void show_columns(std::ostream& stream = std::cout) const;
		//������� ����������� ����������:
		void show_params(std::ostream& stream = std::cout) const;
		//������� ��������� ����� �������:
		size_t ColumnsCnt() const { return cols.size(); }
		//������� ��������� ����� ����������:
		size_t ParamsCnt() const { return params.size(); }
		//��������� ����� ����� ��������/������:
		string getDQLFile() const noexcept(false) { return getFullFileName(Types::DQL, false); }
		string getDMLFile() const noexcept(false) {return getFullFileName(Types::DML, false); }
		//��������� ������ ������ sql-��������:
		vector<string> getDQLFileLst(bool use_sort = true) const noexcept(false) { return getSQLFileLst(Types::DQL); }
		vector<string> getDMLFileLst(bool use_sort = true) const noexcept(false) { return getSQLFileLst(Types::DML); }
		//�������� �������� ����������:
		bool isDQLEmpty() const noexcept(true) { return isEmptyByCode(Types::DQL); }
		bool isDMLEmpty() const noexcept(true) { return isEmptyByCode(Types::DML); }
		//��������� ����� ���������� �����:
		string getTemplateFile() const noexcept(false) { return getFullFileName(Types::Template, false); }
	};

	//��������� ��� ��������� excel-������
	//��������� ��������� � ������� json-������
	//�������� ������ ��� ��������� ������ � ������� excel-������:
	//��������� ����� ����������� � �������������� boost ��� json(�� ���� ������� � ����� �������)
	//�����: https://www.boost.org/doc/libs/1_52_0/doc/html/property_tree.html
	//������� ����� ��� ��������:
	class TIndex
	{
	private:
		size_t index;
		//��������� ������� �� ���� json-������ �� ���������� ����/������
		void setIndexFromJSon(const ptree::value_type& parent_node, const string& tagStr) noexcept(false);
	public:
		static const size_t EmptyIndex = 0;
		//��������� ���������� �������� �� json-�����:
		static string getStrValue(const ptree::value_type& parent_node, const JsonParams& tag) noexcept(true);
		static string getStrValue(const ptree& parent_node, const JsonParams& tag) noexcept(true);
		TIndex(size_t indx = EmptyIndex) : index(indx) {}
		TIndex(const TIndex& x) : index(x.index) {}
		TIndex(const ptree::value_type& parent_node, const string& tagStr) : index(EmptyIndex) { setIndexFromJSon(parent_node, tagStr); }
		TIndex(const ptree::value_type& parent_node, const NS_Const::JsonParams& tag) : index(EmptyIndex) { setByJSon(parent_node, tag); }
		inline bool isEmpty() const noexcept(true) { return index == EmptyIndex; }
		inline size_t get() const noexcept(true) { return index; }
		inline void set(size_t val) noexcept(false) { index = val; }
		void setByJSon(const ptree::value_type& parent_node, const NS_Const::JsonParams& tag) noexcept(true);
		bool operator==(size_t val) const noexcept(true) { return isEmpty() ? false: index == val; }
		bool operator==(const TIndex& val) const noexcept(false) { return isEmpty() ? false : index == val.index; }
		void show(std::ostream& stream = std::cout, const string& front_msg = "") const noexcept(false);
};

	//����� ��� ��������� ���������� ����� excel-�����
	class TSheetData
	{
	private:
		TIndex index;
		TIndex first_row;
		TIndex last_row;
		//��������� �������� �� json-������:
		void setData(const ptree::value_type& parent_node,
			const JsonParams& indx_tag = JsonParams::list_index,
			const JsonParams& first_row_tag = JsonParams::first_row,
			const JsonParams& last_row_tag = JsonParams::last_row) noexcept(true);
	public:
		//�������������
		TSheetData(const ptree::value_type& parent_node);
		//���������������:
		~TSheetData() {}
		//�������� �� �������:
		inline bool isEmpty() const noexcept(true) { return index.isEmpty(); }
		inline bool NoFirstRowIndex() const noexcept(true) { return first_row.isEmpty(); }
		inline bool NoLastRowIndex() const noexcept(true) { return last_row.isEmpty(); }
		//��������� � ��������� ��������:
		inline size_t getListIndex() const noexcept(true) { return index.get(); }
		inline void setListIndex(size_t val) noexcept(true) { index.set(val); }
		//��������� ������� ������ �������:
		inline size_t getStartRow() const noexcept(true) { return NoFirstRowIndex() ? 1 : first_row.get(); }
		inline void setStartRow(size_t val) noexcept(true) { first_row.set(val); }
		//��������� ��������� ������ �� ��������
		inline size_t getLastRow() const noexcept(true) { return last_row.get(); }
		inline void setLastRow(size_t val) noexcept(true) { last_row.set(val); }
		//����������� ��������:
		void show(std::ostream& stream = std::cout) const noexcept(true);

	};

	//����� ��� ��������� �������� ������� ����������:
	class TFilterData
	{
	private:
		TIndex col_indx;//������ �������, ��� ����������� �������
		string value;//�������/�������� ����������
		//��������� �������� �������� �� ���� json-�����
		void setData(const ptree::value_type& parent_node,
			const JsonParams& col_tag = JsonParams::column_index,
			const JsonParams& val_tag = JsonParams::value) noexcept(true);
	public:
		//�����������
		TFilterData(std::pair<size_t, string> val) : col_indx(val.first), value(val.second) {}
		TFilterData(size_t indx, const string& val): col_indx(indx), value(val) {}
		explicit TFilterData(const ptree::value_type& parent_node) { setData(parent_node); }
		//����������
		~TFilterData() {}
		//�������� ������� ��������
		bool isEmpty() const noexcept(true) { return col_indx.isEmpty(); }
		//��������� �������� �������
		size_t getColIndx() const noexcept(true) { return col_indx.get(); }
		//��������� �������
		void setColIndx(size_t val) noexcept(true) { col_indx.set(val); }
		//��������� �������� ������� �������
		inline string getValue() const noexcept(true) { return value; }
		//��������� �������� ��� �������
		inline void setValue(const string& val) noexcept(true) { value = val; }
		//�������� ����������� ��������(�������� �� ����� - ����� ������� �� �����):
		bool operator==(const string& val) const noexcept(true);
		bool operator==(const TFilterData& src) const noexcept(true) { return operator==(src.value); }
		//��������� �������� �������/�������� � ���� ����:
		pair<size_t, string> getPair() const noexcept(true) { return std::make_pair(getColIndx(), getValue()); }
		//����� ������ � �����:
		void show(std::ostream& stream = std::cout) const noexcept(true);
	};

	//��������� ����� ������ ��� ������ ������:
	class TShareData
	{
	private:
		string name;//��� �����
		vector<TSheetData> sh_params;//������ ���������� ��������
		vector<TFilterData> fltr;//������ ��� ����������
		//������� ��������� �������� ���������� �� json-����
		//���������� ������� ��������:
		template <typename Type>
		static void setArrayByJson(const ptree::value_type& node, const JsonParams& tag, 
			vector<Type>& arr) noexcept(false);
		void setData(const ptree::value_type& parent_node,
			const JsonParams& name_tag = JsonParams::name,
			const JsonParams& sheet_tag = JsonParams::Sheet,
			const JsonParams& fltr_tag = JsonParams::filter) noexcept(true);
	public:
		//������������� ������ �����(��� �������� ����� excel), ������� �����(�������������� ������������ excel),
		//����� ������ �������(����������� � �������� excel) � ������ �������� ��������
		TShareData(const ptree::value_type& parent_node, const string& main_path = "");
		//����������
		~TShareData() {}
		//�������� �� �������:
		inline bool isEmpty() const noexcept(true) { return name.empty() || sh_params.empty(); }
		//��������� ����� �����:
		inline string getName() const noexcept(true) { return name; }
		inline void setName(const string& val) noexcept(true) { name = val; }
		//�������� ������� �������:
		inline bool isEmptyFilter() const noexcept(true) { return fltr.empty(); }
		//��������� ������� ���������� �������:
		vector<TSheetData> getSheetParams() const noexcept(true) { return sh_params; }
		const TSheetData& getSheetParam(size_t page) const noexcept(false) { return sh_params[page]; }
		//��������� ������� ����������:
		vector<TFilterData> getFilterLst() const noexcept(true) { return fltr; }
		//��������� �������� ������� �� ������� � �������:
		TFilterData getFilterByIndex(size_t index) const noexcept(true) { return fltr[index]; }
		void setFilter(const vector<TFilterData>& filter) noexcept(true) { fltr = filter; }
		//������� ��������� ����� �������������� �������:
		size_t getPageCnt() const noexcept(true) { return sh_params.size(); }
		void show(std::ostream& stream = std::cout) const noexcept(true);
	};

	//����� ����������� �������������� ���� � �������� ������ � �����:
	class TCellData
	{
	private:
		TIndex dst_indx;//������ ���� � ���������
		TIndex dst_ins_indx;//������ ���� � ���������, ���� ����������� ������
		TIndex src_param_indx;//������ ����-��������� � ���������
		TIndex src_val_indx;//������ ����-�������� � ���������
		DataType in_data_type;//��� ������ �� ����� �� ������ dst_index
		DataType out_data_type;//��� ������ �� ������ �� ������ src_val_indx;
		//������� ��������� ���� ������ �� json-������ �� ����:
		static DataType getTypeFromJsonByCode(const ptree::value_type& node, const JsonParams& tag) noexcept(true);
	protected:
		//������������� �� ���������:
		TCellData(size_t idst = TIndex::EmptyIndex, size_t ins_dst = TIndex::EmptyIndex, 
			size_t isrc = TIndex::EmptyIndex, size_t val_src = TIndex::EmptyIndex, 
			const DataType& in_type = DataType::ErrorType, 
			const DataType& out_type = DataType::ErrorType) :
			dst_indx(idst), dst_ins_indx(ins_dst), src_param_indx(isrc), src_val_indx(val_src),
			in_data_type(in_type), out_data_type(out_type) {}
		//��������� ��������:
		TCellData& setData(size_t dst, size_t ins, size_t src_param, size_t val) noexcept(true);
		TCellData& setData(const ptree::value_type& parent_node,
			const JsonParams& dst_tag = JsonParams::dst_index,
			const JsonParams& dst_ins_tag = JsonParams::dst_insert_index,
			const JsonParams& src_param_tag = JsonParams::src_param_index,
			const JsonParams& src_val_tag = JsonParams::src_val_index,
			const JsonParams& in_type = JsonParams::in_data_type,
			const JsonParams& out_type = JsonParams::out_data_type
			) noexcept(true);
	public:
		//������������� �� ������ �� json-������
		explicit TCellData(const ptree::value_type& parent_node) { setData(parent_node); }
		//���������� �� ���� �� ������
		~TCellData() {}
		//�������� �� ������� ��������
		bool EmptyDstIndx() const noexcept(true) { return dst_indx == TIndex::EmptyIndex; }
		bool EmptyInsIndx() const noexcept(true) { return dst_ins_indx == TIndex::EmptyIndex; }
		bool EmptySrcParam() const noexcept(true) { return src_param_indx == TIndex::EmptyIndex; }
		bool EmptySrcVal() const noexcept(true) { return src_val_indx == TIndex::EmptyIndex; }
		bool isEmpty() const noexcept(true) { return EmptyDstIndx(); }
		//����� ������:
		inline size_t DstIndex() const noexcept(true) { return dst_indx.get(); }
		inline size_t InsIndex() const noexcept(true) { return dst_ins_indx.get(); }
		inline size_t SrcParam() const noexcept(true) { return src_param_indx.get(); }
		inline size_t SrcVal() const noexcept(true) { return src_val_indx.get(); }
		//�������� ����������:
		inline void setDstIndex(size_t val) noexcept(true) { dst_indx.set(val); }
		inline void setInsIndex(size_t val) noexcept(true) { dst_ins_indx.set(val); }
		inline void setSrcParam(size_t val) noexcept(true) { src_param_indx.set(val); }
		inline void setSrcVal(size_t val) noexcept(true) { src_val_indx.set(val); }
		//������� ��������� ���� ��� �������� ���������:
		DataType getInType() const noexcept(true) { return in_data_type; }
		//������� ��������� ���� ��� ��������� ���������:
		DataType getOutType() const noexcept(true) { return out_data_type; }
		//����������
		TCellData& operator=(const TCellData& cd) noexcept(true);
		//�����������:
		void show(std::ostream& stream = std::cout) const noexcept(true);
	};

	//����� ����� ��������� �����
	class TCellMethod
	{
	private:
		JSonMeth code;//��� ������ ���������
		TColor color_if_found;//���� ���������, ���� ������ �������
		TColor color_not_found;//���� ���������, ���� ������ �� �������
		//��������� ��������:
		void setMethod(size_t meth, size_t find_color, size_t not_find_color) noexcept(true);
		//��������� ������ �� ���� JSon-�����
		void setMethod(const ptree::value_type& node,	const JsonParams& code_tag = JsonParams::code,
			const JsonParams& color_find_tag = JsonParams::color_if_found,
			const JsonParams& color_not_found_tag = JsonParams::color_not_found);
	public:
		//������������� �� ���������
		TCellMethod(const JSonMeth& meth = JSonMeth::Null, const TColor& find_color = TColor::COLOR_WHITE,
			const TColor& not_find_color = TColor::COLOR_WHITE) :
			code(meth), color_if_found(find_color), color_not_found(not_find_color) {}
		//�������������
		explicit TCellMethod(ptree& parent_node, const JsonParams& tag_meth = JsonParams::Method);
		explicit TCellMethod(const ptree::value_type& parent_node) : code(JSonMeth::Null), color_if_found(TColor::COLOR_WHITE),
			color_not_found(TColor::COLOR_WHITE) { setMethod(parent_node); }
		~TCellMethod() {}
		//�������� �� �������:
		bool isEmpty() const noexcept(true) { return code == JSonMeth::Null; }
		bool isEmptyIncludeColor() const noexcept(true) { return color_if_found == TColor::COLOR_WHITE; }
		bool isEmptyExcludeColor() const noexcept(true) { return color_not_found == TColor::COLOR_WHITE; }
		bool isEmptyColor() const noexcept(true) { return isEmptyExcludeColor() and isEmptyIncludeColor(); }
		//��������� ������ ���������
		JSonMeth getMethod() const noexcept(true) { return code; }
		//��������� ����� ������� ��� ���������� �������:
		TColor getIncludeColor() const noexcept(true) { return color_if_found; }
		//��������� ����� ������� ��� �� ���������� �������:
		TColor getExcludeColor() const noexcept(true) { return color_not_found; }
		//��������� ������:
		void setMethod(const JSonMeth& val) noexcept(true) { code = val; }
		//��������� ������:
		void setColotInclude(const TColor& val) noexcept(true) { color_if_found = val; }
		void setColorExclude(const TColor& val) noexcept(true) { color_not_found = val; }
		//����� ������ � �����:
		void show(std::ostream& stream = std::cout) const noexcept(true);
	};


	//��������� �������������� �����:
	class TProcCell
	{
	private:
		TCellMethod meth;//����� ���������
		TShareData* SrcFile;//����-��������
		vector<TUserTune> db_tune;//�������� ��� ���������� � ��
		vector<TCellData> cel_arr;//������� ������������� �������
		//������������� �����-���������
		void InitSrcFile(ptree& node, const JsonParams& tag = JsonParams::SrcFile) noexcept(true);
		//��������������� �����-���������
		void DeInitSrcFile() noexcept(false) { if (SrcFile) delete SrcFile; SrcFile = nullptr; }
		//������������� �������� ���������� � ��
		void InitDBTune(const ptree& node, const TSimpleTune* tune_ref,
			const JsonParams& tag = JsonParams::DB_Config) noexcept(true);
		//������������� ������� ������ � ��������/��������:
		void InitCellData(ptree& node, const JsonParams& tag = JsonParams::DataArr) noexcept(true);
		//������������� � ����������� �� ������:
		void InitByMethod(ptree& node, const TSimpleTune* tune_ref = nullptr) noexcept(true);
	public:
		//�������������
		explicit TProcCell(ptree& parent_node, const TSimpleTune* tune_ref = nullptr);
		//���������������
		~TProcCell() { DeInitSrcFile(); }
		//�������� �� �������:
		bool isEmpty() const noexcept(true) { return meth.isEmpty() or cel_arr.empty(); }
		//������ ���������� �� ������:
		JSonMeth getMethodCode() const noexcept(true) { return meth.getMethod(); }
		bool NoColors() const noexcept(true) { return meth.isEmptyColor(); }
		TColor getIncludeColor() const noexcept(true) { return meth.getIncludeColor(); }
		TColor getExcludeColor() const noexcept(true) { return meth.getExcludeColor(); }
		//��������� ������ �� ����� ���������:
		bool NoSrcFile() const noexcept(true) { return SrcFile == nullptr or SrcFile->isEmpty(); }
		const TShareData* getSrcFilRef() const noexcept(true) { return SrcFile; }
		string SrcFileName() const noexcept(true) { return NoSrcFile() ? string() : SrcFile->getName(); }
		size_t TuneCnt() const noexcept(true) { return db_tune.size(); }
		size_t CellCnt() const noexcept(true) { return cel_arr.size(); }
		vector<TFilterData> SrcFileFilters() const noexcept(true) { return NoSrcFile() ? vector<TFilterData>() : SrcFile->getFilterLst(); }
		//��������� ������� ������ ��� �����:
		vector<TCellData> getCellDataArr() const noexcept(true) { return cel_arr; }
		//��������� �������� ��� ���������� � ��
		vector<TUserTune> getDBTuneArr() const noexcept(true) { return db_tune; }
		//����������� ������:
		void show(std::ostream& stream = std::cout) const noexcept(true);
	};

	//��������� ������ �� ������������ ������ ��������� � ���������
	class TExcelProcData
	{
	private:
		TShareData* DstFile;//����� ���������
		TProcCell* cells;//������� �������������� ��������
		//������������� �����-���������:
		void InitDstFile(const ptree::value_type& node, const string& main_path) noexcept(true);
		//�������������� �����-���������
		void DeInitDstFile() noexcept(true);
		//������������� ������ � �������:
		void InitCells(ptree::value_type& node, const TSimpleTune* tune_ref = nullptr) noexcept(true);
		//��������������� ������ �����
		void DeInitCells() noexcept(true);
		//������������� ������� �� json-����� �� �������� ����:
		bool InitObjByTag(ptree& json, const JsonParams& tag, const TSimpleTune* tune_ref = nullptr) noexcept(false);
		//������������� ���������� �������:
		void InitExcelProcData(const string& json_file, const TSimpleTune* tune_ref = nullptr) noexcept(true);
	public:
		//������������� json-������
		explicit TExcelProcData(const string& json_file, const TSimpleTune* tune_ref = nullptr);
		//���������������
		~TExcelProcData();
		//�������� �� �������:
		bool isDstFileEmpty() const noexcept(true) { return DstFile == nullptr or DstFile->isEmpty(); }
		bool isCellsEmpty() const noexcept(true) { return cells == nullptr or cells->isEmpty(); }
		bool isEmpty() const noexcept(true) { return isDstFileEmpty() or isCellsEmpty(); }
		//��������� ���� ������ ���������:
		JSonMeth MethodCode() const noexcept(true) { return cells ? cells->getMethodCode() : JSonMeth::Null; }
		//��������� ������ �� ����-��������:
		TShareData& getDstFile() const noexcept(false) { return *DstFile; }
		//��������� ������ �� ������ �� ��������:
		TProcCell& getCellsData() const noexcept(false) { return *cells; }
		//��������� ����� �������������� �������:
		size_t getProcPagesCnt() const noexcept(true) { return DstFile ? DstFile->getPageCnt() : 0; }
		//����������� ������:
		void show(std::ostream& stream = std::cout) const noexcept(true);
	};
/**/
}

#endif TUNE_PARAM_H_