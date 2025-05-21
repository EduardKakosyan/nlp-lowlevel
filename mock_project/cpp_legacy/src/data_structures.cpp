#include "../include/data_structures.hpp"
#include <iostream>

/**
 * @brief Prints a formatted report to the standard output stream
 * 
 * This function takes a ReportGenerator object and prints its content
 * in a formatted way with header and footer separators. The report
 * content is obtained by calling the generateReportContent() method
 * of the ReportGenerator object.
 * 
 * @param report A constant reference to the ReportGenerator object to be printed
 * @return void
 * 
 * @note This function assumes that generateReportContent() is either
 *       a const method or that the ReportGenerator object is non-const
 */
void printReport(const ReportGenerator& report) {
    std::cout << "--- Printing Report --- \n";
    std::cout << report.generateReportContent(); // Assuming generateReportContent is const or report is non-const
    std::cout << "--- End of Report --- \n";
}

// Adding const to the definition of generateReportContent for ReportGenerator
// This part was missing from the file, assuming it should be here based on the hpp
// If ReportGenerator::generateReportContent() definition is elsewhere, this will add it.
// Based on the structure, it seems the definition was previously in the .hpp, which is unusual for separate compilation.
// The following is the definition from the .hpp file, now made const and moved to the .cpp file.

std::string ReportGenerator::generateReportContent() const { // Added const
    std::string content = "Report: " + report_title + "\n";
    content += "Status Codes Mapping:\n";
    for (const auto& pair : status_codes) {
        content += std::to_string(pair.first) + ": " + pair.second + "\n";
    }
    content += "Entries:\n";
    int count = 1;
    for (const auto& entry : entries) {
        content += std::to_string(count++) + ". " + entry + "\n";
    }
    return content;
}