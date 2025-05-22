#include <fstream>
#include "1_Quant.h"

void TestsToFile() {

    std::ofstream out("output.txt");
    
    out << "\nGrover's Algorithm Tests with Specific Initial States:\n";

    for (int test_case = 1; test_case <= 3; test_case++) {
        out << "\n--- Test Case " << test_case << " ---\n";
        Qubit q1, q2;
    
        switch(test_case) {
            case 1:
                q1.setState(7, 200);    
                q2.setState(5, 4);    
                break;
            case 2:
                q1.setState(1, 1); 
                q2.setState(1, 1);  
                break;
            case 3:
                q1.setState(0.5, sqrt(3)/2);         
                q2.setState(sqrt(2)/2, sqrt(2)/2);   
                break;
        }

        out << "Initial state q1: " << q1.toString() << "\n";
        out << "Initial state q2: " << q2.toString() << "\n";

        groverSearch(q1, q2);

        out << "After Grover iteration q1: " << q1.toString() << "\n";
        out << "After Grover iteration q2: " << q2.toString() << "\n";

        const State& final_s1 = q1.getState();
        const State& final_s2 = q2.getState();
    
        double p00 = std::norm(final_s1[0] * final_s2[0]);
        double p01 = std::norm(final_s1[0] * final_s2[1]);
        double p10 = std::norm(final_s1[1] * final_s2[0]);
        double p11 = std::norm(final_s1[1] * final_s2[1]);

        out << "Final probabilities:\n";
        out << "|00⟩: " << p00 << "\n";
        out << "|01⟩: " << p01 << "\n";
        out << "|10⟩: " << p10 << "\n";
        out << "|11⟩: " << p11 << "\n";
    
        if (p11 > p00 && p11 > p01 && p11 > p10) {
            out << "SUCCESS: |11⟩ has the highest probability (" << p11 << ")\n";
        } else {
            out << "WARNING: |11⟩ doesn't have the highest probability\n";
        }
    }
    out.close();
    
}