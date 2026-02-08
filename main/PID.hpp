namespace pid {
    const double p = 0.0;
    const double i = 0.0;
    const double d = 0.0;

    static double iError = 0.0;
    const double iMax = 10.0;

    static double lastError = 0.0;

    double next_pid(double error, double delta, bool accumulateI = true) {
        double pTerm = p * error;
        
        if (accumulateI) {
            iError += error;
            iError = iError > iMax ? iMax : iError;
        }

        double iTerm = iError * i;

        double dTerm = d * ((error - lastError) / delta);
        lastError = error;

        return pTerm + iTerm + dTerm;
    }
}