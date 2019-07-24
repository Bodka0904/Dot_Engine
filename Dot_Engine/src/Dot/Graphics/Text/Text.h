

namespace Dot {

	class Text
	{
	public:
		Text(const std::string& text, int x,int y, int size);
		~Text();

		void PrintText(const std::string& name);


	private:
		unsigned int m_VAO;
		unsigned int m_VertSize;

	};

}