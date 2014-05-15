#ifndef __DRAGON_CORE_H__
#define __DRAGON_CORE_H__

#include "Macro.h"
#include <cstdio>
#include <cctype>
#include <cassert>
#include <string>

NS_DRAGON

		struct IStream {
			virtual int getChar() = 0;
			virtual void ungetChar(int ch) = 0;
			virtual bool isEOF() = 0;
		};

		class CharStream : public IStream {
			std::string s;
			unsigned idx;

		public:
			CharStream(std::string str) : s(str), idx(0) {}

			int getChar()
			{
				return s[idx++];
			}

			void ungetChar(int ch)
			{
				idx--;
			}

			bool isEOF()
			{
				return (idx == (s.length() - 1)) ? true : false;
			}

			std::string getString()
			{
				return s;
			}
		};

		class FileStream : public IStream 
		{
			FILE *_fConfig;

		public:
			FileStream(const char *ConfigPath)
			{
				_fConfig = fopen(ConfigPath, "r");
			}

			~FileStream()
			{
				if (_fConfig != NULL)
					fclose(_fConfig);
			}
			
			int getChar()
			{
				assert(_fConfig);
				return fgetc(_fConfig);
			}

			void ungetChar(int ch)
			{
				assert(_fConfig);
				ungetc(ch, _fConfig);
			}

			bool isEOF()
			{
				return true;
			}
		};

		class ASTree {
		public:
			virtual ~ASTree() {}
		};

		/*
				String Example: 
		*/
		class StringASTree : public ASTree {
		protected:
			std::string _str;
		public:
			StringASTree(std::string str) : _str(str) {} 
			virtual ~StringASTree() {}

			std::string getString()
			{
				return _str;
			}

		};

		/*

		*/
		class IntegerASTree : public ASTree {
		protected:
			int _i;
		public:
			IntegerASTree(int i) : _i(i) {}

			int getInteger()
			{
				return _i;
			}
		};

		/*

		*/
		class FloatASTree : public  ASTree	 {
		protected:
			double _f;
		public:
			FloatASTree(double f) : _f(f) {}
		};

		/*

		*/
		class BinOpASTree : public ASTree {
		protected:
			char _op;
			ASTree *_lchild;
			ASTree *_rchild;

		public:
			BinOpASTree(char op, ASTree *left, ASTree *right) 
				: _op(op), _lchild(left), _rchild(right) {}

			char getOp()
			{
				return _op;
			}

			ASTree *getLChild()
			{
				return _lchild;
			}

			ASTree *getRChild()
			{
				return _rchild;
			}
		};

		class Tokenizer {
		protected:
			int _lastChar;
			int _lastTok;
			
		public:
			virtual int getNextTok() = 0;

			int getLastChar()
			{
				return _lastChar;
			}

			int getLastTok()
			{
				return _lastTok;
			}
		};

		class Parser {

		};
NS_END

#endif

