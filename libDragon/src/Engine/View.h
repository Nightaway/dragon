#ifndef __VIEW_H__
#define __VIEW_H__

#include <string>
#include <map>

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
				return (idx >= (s.length() - 1)) ? true : false;
			}

			std::string getString()
			{
				return s;
			}
		};

	typedef std::map<std::string, std::string> ViewBag;
	class ViewParser {
		CharStream s;
		ViewBag &viewBag;

	public:
		ViewParser(std::string str, ViewBag &bag) : s(str), viewBag(bag) {}
		std::string parse(char endCh);
	};

NS_END

#endif