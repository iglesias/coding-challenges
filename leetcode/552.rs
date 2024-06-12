impl Solution {
    pub fn check_record(n: i32) -> i32 {
        let mut record_i: [i32; 6] = [1, 1, 0, 1, 0, 0];
        let modulo = 1000000000 + 7;
        for i in 2..(n+1) {
            let mut record_j: [i32; 6] = [0; 6];
            record_j[0] = (((record_i[0] + record_i[1]) % modulo) + record_i[2]) % modulo;
            record_j[1] = record_i[0];
            record_j[2] = record_i[1];
            record_j[3] = 0;
            for i in 0..6 {
                record_j[3] = (record_j[3] + record_i[i]) % modulo;
            }
            record_j[4] = record_i[3];
            record_j[5] = record_i[4];
            record_i = record_j;
        }
        let mut ans: i32 = 0;
        for i in 0..6 {
            ans = (ans + record_i[i]) % modulo;
        }
        ans
    }
}
