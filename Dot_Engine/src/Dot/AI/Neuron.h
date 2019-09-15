#pragma once



namespace Dot {

	typedef std::vector<Neuron> Layer;

	class Neuron
	{
	public:
		struct Connection
		{
			double weight;
			double deltaWeight;
		};

		Neuron(unsigned int numOutputs, unsigned int myIndex);

		void FeedForward(const Layer& preLayer);
		void CalcOutputGradients(double targetVal);
		void CalcHiddenGradients(const Layer& nextLayer);
		void UpdateInputWeights(Layer& prevLayer);
	
		void SetOutPutVal(double val) { m_OutputVal = val; }
		double GetOutputVal() const { return m_OutputVal; }

	private:
		static double transferFunction(double x);
		static double transferFunctionDerivative(double x);
		static double randomWeight(void) { return rand() / double(RAND_MAX); }
		double sumDOW(const Layer& nextLayer) const;

	private:
		unsigned int m_Index;

		double m_OutputVal;
		double m_Gradient;
		std::vector<Connection> m_OutputWeights;

	private:
		static double eta;   // [0.0..1.0] overall net training rate
		static double alpha; // [0.0..n] multiplier of last weight change (momentum)
	};

}