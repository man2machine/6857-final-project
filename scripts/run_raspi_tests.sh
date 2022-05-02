raspi_test_crypto = $(find ../raspi_tests/crypto -type f -iname "*.cpp" | sed "s/.*\///; s/\.cpp//")
raspi_test_crypto_lw = $(find ../raspi_tests/crypto_lw -type f -iname "*.cpp" | sed "s/.*\///; s/\.cpp//")
raspi_test_lwc_finalists = $(find ../raspi_tests/lwc_finalists -type f -iname "*.cpp" | sed "s/.*\///; s/\.cpp//")

printf raspi_test_crypto
