package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"strings"
)

type Direction struct {
	x, y int
}

var DIRECTIONS = []Direction{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}

func solve(line string) int {
	position  := [2]int{0, 0}
	heading   := [2]int{0, 1}
	direction := 0

	for _, item := range strings.Split(line, ",") {
		item = strings.TrimSpace(item)
		letter := item[0]
		digit := 0
		fmt.Sscanf(item[1:], "%d", &digit)
		adder := 1
		if letter == 'L' {
			adder = -1
		}
		direction = (direction + adder + 4) % 4
		heading = [2]int{DIRECTIONS[direction].x, DIRECTIONS[direction].y}
		position[0] += digit * heading[0]
		position[1] += digit * heading[1]
	}

	return int(math.Abs(float64(position[0])) + math.Abs(float64(position[1])))
}

func main() {
	reader   := bufio.NewReader(os.Stdin)
	input, _ := reader.ReadString('\n')
	fmt.Println(solve(input))
}

func testSolve() {
	tests := []struct {
		input  string
		output int
	}{
		{"R2, L3", 5},
		{"R2, R2, R2", 2},
		{"R5, L5, R5, R3", 12},
	}

	for _, test := range tests {
		result := solve(test.input)
		if result == test.output {
			fmt.Printf("PASS: Input: %s, Output: %d\n", test.input, test.output)
		} else {
			fmt.Printf("FAIL: Input: %s, Expected: %d, Got: %d\n", test.input, test.output, result)
		}
	}
}
