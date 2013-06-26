#include "../Macro.h"

#include "View.h"

USING_NS_DRAGON;

std::string ViewParser::parse(char endCh)
{
	char ch; 
	std::string Out;
	while (!s.isEOF() && (ch = (char)s.getChar()))
	{
			// 变量开始
			if (ch == '$') 
			{
				std::string varible;
				ch = s.getChar();
				// 解析${name}格式变量
				if (ch == '{')
				{
					ch = s.getChar();
					while (ch != '}') 
					{
						varible += ch;
						ch = s.getChar();
					}
					Out += viewBag[varible];
				}
				// 解析if 语句
				else if (ch == '[')
				{
					std::string varible_if;
					std::string varible_c;
			
					ch = s.getChar();
					if (ch == '$')
					{
						ch = s.getChar();
						while (ch != ' ' && ch != '	') 
						{
							varible_if += ch;
							ch = s.getChar();
						}

						if (viewBag[varible_if].empty())	
						{
							Out += "";
						}  else 	{

							while (	(ch = s.getChar()) != ']')
							{
								if (ch == '$')
								{
										std::string varible;
										ch = s.getChar();
										// 解析${name}格式变量
										if (ch == '{')
										{
											ch = s.getChar();
											while (ch != '}') 
											{
												varible += ch;
												ch = s.getChar();
											}
											Out += viewBag[varible];
										} else {
													std::string varible;
													 /*
																变量名必须是字母开头
													*/
													if (::isalpha(ch)) 
													{
														varible += ch;
														ch = s.getChar();

														/*
																变量名之后必须是字母数字或下划线
														*/
														while (::isalnum(ch)) 
														{
															varible += ch;
															ch = s.getChar();
														}
														s.ungetChar(ch);

														Out += viewBag[varible];
													}
										}
								} else {
										Out += ch;
								}
							}
						}

						while (ch != ']')
							ch = s.getChar();
					}
				}	else	{
					 /*
								变量名必须是字母开头
					*/
					if (::isalpha(ch)) 
					{
						varible += ch;
						ch = s.getChar();

						/*
								变量名之后必须是字母数字或下划线
						*/
						while (::isalnum(ch)) 
						{
							varible += ch;
							ch = s.getChar();
						}
						s.ungetChar(ch);

						Out += viewBag[varible];
					}// end if
				}
		}	else 
		{
			if (endCh != '$')
			{
				if (ch == endCh)
					continue;
			}
			 Out += ch;
		}

	} //  end while

	return Out;
} 