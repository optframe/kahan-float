
bazel coverage -s   --instrument_test_targets   --experimental_cc_coverage   --combined_report=lcov   --coverage_report_generator=@bazel_tools//tools/test/CoverageOutputGenerator/java/com/google/devtools/coverageoutputgenerator:Main   //...

genhtml -o bazel-out/_coverage/reports/ --ignore-errors source bazel-out/_coverage/_coverage_report.dat

google-chrome bazel-out/_coverage/reports/index.html 
