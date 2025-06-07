#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to run a test and check its output
run_test() {
    local test_file=$1
    local expected_output=$2
    local test_name=$(basename "$test_file" .b)
    
    echo "Running test: $test_name"
    
    # Special handling for IO test
    if [[ "$test_name" == "io_test" ]]; then
        # Create a temporary file with just a newline
        echo -e "\n" > temp_input.txt
        printf "\x04" >> temp_input.txt
        # Run the test with the input file
        output=$(../brainfuck "$test_file" < temp_input.txt 2>&1)
        # Check if the output matches the expected pattern
        if [[ $(echo "$output" | wc -l) -eq 2 ]] && \
           [[ $(echo "$output" | head -n1) == $(echo "$output" | tail -n1) ]] && \
           [[ $(echo "$output" | head -n1) =~ ^[A-Z]{2}$ ]]; then
            # Check if the output indicates proper newline handling (no 'O' in output)
            if [[ $(echo "$output" | head -n1) != *"O"* ]]; then
                echo -e "${GREEN}✓ Test passed: $test_name${NC}"
                echo "Output indicates: $(echo "$output" | head -n1)"
                return 0
            else
                echo -e "${RED}✗ Test failed: $test_name${NC}"
                echo "Newline handling incorrect (output contains 'O')"
                echo "Got: $output"
                return 1
            fi
        else
            echo -e "${RED}✗ Test failed: $test_name${NC}"
            echo "Expected: Two identical lines of two uppercase letters"
            echo "Got: $output"
            return 1
        fi
    else
        # Run the test normally
        output=$(../brainfuck "$test_file" 2>&1)
    fi
    exit_code=$?
    
    # Check if the test passed
    if [[ "$test_name" == "unmatched_bracket"* ]]; then
        # For bracket tests, we expect an error
        if [[ $exit_code -ne 0 ]] && [[ "$output" == *"Unmatched"* ]]; then
            echo -e "${GREEN}✓ Test passed: $test_name${NC}"
            return 0
        else
            echo -e "${RED}✗ Test failed: $test_name${NC}"
            echo "Expected: Error about unmatched bracket"
            echo "Got: $output"
            return 1
        fi
    elif [[ "$test_name" == "array_size_test" ]]; then
        # For array size test, we expect a single '#'
        if [[ "$output" == "#" ]]; then
            echo -e "${GREEN}✓ Test passed: $test_name${NC}"
            return 0
        else
            echo -e "${RED}✗ Test failed: $test_name${NC}"
            echo "Expected: #"
            echo "Got: $output"
            return 1
        fi
    elif [[ "$test_name" == "obscure_test" ]]; then
        # For obscure problems test, we expect 'H'
        if [[ "$output" == "H" ]]; then
            echo -e "${GREEN}✓ Test passed: $test_name${NC}"
            return 0
        else
            echo -e "${RED}✗ Test failed: $test_name${NC}"
            echo "Expected: H"
            echo "Got: $output"
            return 1
        fi
    elif [[ "$test_name" == "bounds1_test" ]]; then
        # For left bounds test, we expect no output
        if [[ "$output" == "Error: Pointer moved out of bounds" ]]; then
            echo -e "${GREEN}✓ Test passed: $test_name${NC}"
            return 0
        else
            echo -e "${RED}✗ Test failed: $test_name${NC}"
            echo "Expected: (empty)"
            echo "Got: $output"
            return 1
        fi
    elif [[ "$test_name" == "bounds2_test" ]]; then
        # For right bounds test, we expect a series of '!' characters and an out of bounds error
        if [[ "$output" == *"Pointer moved out of bounds"* ]] && \
           [[ $(echo "$output" | grep -o '!' | wc -l) -eq 29999 ]]; then
            echo -e "${GREEN}✓ Test passed: $test_name${NC}"
            return 0
        else
            echo -e "${RED}✗ Test failed: $test_name${NC}"
            echo "Expected: 29999 '!' characters followed by out of bounds error"
            echo "Got: $output"
            return 1
        fi
    fi
}

# Make sure we're in the tests directory
cd "$(dirname "$0")"

# Check if brainfuck executable exists
if [[ ! -f "../brainfuck" ]]; then
    echo "Error: brainfuck executable not found. Please build the project first."
    exit 1
fi

# Run all .b files in the current directory
failed_tests=0
for test_file in *.b; do
    if [[ -f "$test_file" ]]; then
        run_test "$test_file"
        if [[ $? -ne 0 ]]; then
            failed_tests=$((failed_tests + 1))
        fi
        echo "----------------------------------------"
    fi
done

# Print summary
if [[ $failed_tests -eq 0 ]]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}$failed_tests test(s) failed${NC}"
    exit 1
fi 
