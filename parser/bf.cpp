/**
 * This example is not working, but it has a lot of code that could be useful :)
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <memory>

namespace FileUtils {
	std::string readFile(const std::string& filename);	
}

namespace BF {
	namespace Token {
		class Token;
	  class Inc;
	  class Dec;
		class MoveLeft;
		class MoveRight;
		class Print;
		// class LoopLeft;
		// class LoopRight;
	}

	class AbstractBFProgram;
	class BFProgram;

	namespace Parser {
		using TokenStream = std::vector<std::unique_ptr<Token::Token>>;

		TokenStream parseFile(const std::string& filename);
		TokenStream parse(const std::string& source);
	}

	void run(const Parser::TokenStream& tokenStream);
}

namespace BF {

	class AbstractBFProgram {
	public:
		virtual void exec(const Token::Inc& t) = 0;
		virtual void exec(const Token::Dec& t) = 0;
		virtual void exec(const Token::MoveLeft& t) = 0;
		virtual void exec(const Token::MoveRight& t) = 0;
		virtual void exec(const Token::Print& t) = 0;
		// virtual void exec(const Token::LoopLeft& t) = 0;
		// virtual void exec(const Token::LoopRight& t) = 0;
	};

	class BFProgram: public AbstractBFProgram {
	protected:
		int pos;
		std::vector<int> tape;
	public:
		BFProgram(): pos(0) {
			tape.push_back(0);
		}

		void exec(const Token::Inc& t) override {
			tape[pos]++;
		}

		void exec(const Token::Dec& t) override {
			tape[pos]--;
		}

		void exec(const Token::MoveLeft& t) override {
			if (pos == 0) {
				std::cerr << "Não é possível decrementar mais que 0\n";
				exit(1);
			}
			pos--;
		}

		void exec(const Token::MoveRight& t) override {
			pos++;
			tape.push_back(0);
		}

		void exec(const Token::Print& t) override {
			std::cout << tape[pos];
		}

		// void exec(const Token::LoopLeft& t) override {

		// }

		// void exec(const Token::LoopRight& t) override {
			
		// }
	};

	namespace Token {
		enum Type {
			INC = '+',
			DEC = '-',
			MOVE_RIGHT = '>',
			MOVE_LEFT = '<',
			PRINT = '.',
			LOOP_LEFT = '[',
			LOOP_RIGHT = ']'
		};

		class Token {
		protected:
			Type type;

		public:
			virtual void print() const = 0;
			virtual void accept(BF::AbstractBFProgram& program) const = 0;

			Type getType() const {
				return this->type;
			}
		};

		class Inc: public Token {
		public:
			Inc() {
				this->type = INC;
			}

			void print() const override {
				std::cout << "<INC>";
			}

			void accept(BF::AbstractBFProgram& program) const {
				program.exec(*this);
			}
		};

		class Dec: public Token {
		public:
			Dec() {
				this->type = DEC;
			}

			void print() const override {
				std::cout << "<DEC>";
			}

			void accept(BF::AbstractBFProgram& program) const {
				program.exec(*this);
			}
		};

		class MoveRight: public Token {
		public:
			MoveRight() {
				this->type = MOVE_RIGHT;
			}

			void print() const override {
				std::cout << "<MOVE_RIGHT>";
			}

			void accept(BF::AbstractBFProgram& program) const {
				program.exec(*this);
			}
		};

		class MoveLeft: public Token {
		public:
			MoveLeft() {
				this->type = MOVE_LEFT;
			}

			void print() const override {
				std::cout << "<MOVE_LEFT>";
			}

			void accept(BF::AbstractBFProgram& program) const {
				program.exec(*this);
			}
		};

		class Print: public Token {
		public:
			Print() {
				this->type = PRINT;
			}

			void print() const override {
				std::cout << "<PRINT>";
			}

			void accept(BF::AbstractBFProgram& program) const {
				program.exec(*this);
			}
		};

		// class LoopLeft: public Token {
		// public:
		// 	LoopLeft() {
		// 		this->type = LOOP_LEFT;
		// 	}

		// 	void print() const override {
		// 		std::cout << "<LOOP_LEFT>";
		// 	}

		// 	void accept(BF::AbstractBFProgram& program) const {
		// 		program.exec(*this);
		// 	}
		// };

		// class LoopRight: public Token {
		// public:
		// 	LoopRight() {
		// 		this->type = LOOP_RIGHT;
		// 	}

		// 	void print() const override {
		// 		std::cout << "<LOOP_RIGHT>";
		// 	}

		// 	void accept(BF::AbstractBFProgram& program) const {
		// 		program.exec(*this);
		// 	}
		// };
	} // namespace Token

  namespace Parser {

		TokenStream parseFile(const std::string& filename) {
			std::string fileContent = FileUtils::readFile(filename);
			return parse(fileContent);
		}

		TokenStream parse(const std::string& source) {
			TokenStream tokenStream;
			tokenStream.reserve(source.size());

			size_t pos = 0;
			for(const auto& c: source) {
				pos++;

				switch(c) {
					case Token::INC:
						tokenStream.emplace_back(new Token::Inc);
						break;

					case Token::DEC:
						tokenStream.emplace_back(new Token::Dec);
						break;

					case Token::MOVE_LEFT:
						tokenStream.emplace_back(new Token::MoveLeft);
						break;

					case Token::MOVE_RIGHT:
						tokenStream.emplace_back(new Token::MoveRight);
						break;
						
					case Token::PRINT:
						tokenStream.emplace_back(new Token::Print);
						break;
						
					case Token::LOOP_LEFT: {
						// tokenStream.emplace_back(new Token::LoopLeft);
						// TODO Substituir std::string por iterators mesmo... =/
						std::string sub = source.substr(pos, source.size());
						TokenStream subStream = parse(sub);

						// tokenStream.insert(std::end(tokenStream), std::begin(subStream), std::end(subStream));
						std::for_each(std::begin(subStream), std::end(subStream), [&tokenStream](auto &x) {
							tokenStream.push_back(std::move(x));
							// tokenStream.emplace_back(std::move(x));
						});
						break;
					}
						
					case Token::LOOP_RIGHT:
						// tokenStream.emplace_back(new Token::LoopRight);
						// TODO Tratar o caso de sair do laço na recursão... [...]
						return tokenStream;
						// break;
				}
			}

			return tokenStream;
		}
	} // namespace Parser

	void run(const Parser::TokenStream& tokenStream) {
		BF::BFProgram program;

		for (const auto& token: tokenStream) {
			token->accept(program);
		}
	}
} // namespace BF

namespace FileUtils {

	std::string readFile(const std::string& filename) {
		std::ifstream fileStream;

		// Set exceptions to be thrown on failure
		fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// May throw now if reading fails.
			fileStream.open(filename);

			std::stringstream ss;
			ss << fileStream.rdbuf();
			return ss.str();

		} catch (const std::system_error& e) {
			std::cerr
			  << "It was not possible to read file '" << filename << "'\n"
			  << "Code: " << e.code() << '\n'
			  << "Message: " << e.what() << '\n';

			exit(EXIT_FAILURE);
		}
	}
} // namespace FileUtils

int main(int argc, char *argv[])
{
	assert(argc == 2);

	std::string filename{argv[1]};

	auto tokenStream = BF::Parser::parseFile(filename);
	BF::run(tokenStream);

	std::cout << '\n';
}
