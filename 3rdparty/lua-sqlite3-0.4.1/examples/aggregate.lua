
require "path"
require "sqlite3"


local db = sqlite3.open_memory()

assert( db:exec[[

  CREATE TABLE test (col1, col2);

  INSERT INTO test VALUES (1, 2);
  INSERT INTO test VALUES (2, 4);
  INSERT INTO test VALUES (3, 6);
  INSERT INTO test VALUES (4, 8);
  INSERT INTO test VALUES (5, 10);

]] )


assert( db:set_aggregate("my_stats", 2, function()

  local square_error_sum = 0

  local function step(a, b)
    local error        = a - b
    local square_error = error * error
    square_error_sum   = square_error_sum + square_error
  end

  local function final(num_called)
    return square_error_sum / num_called
  end

  return step, final

end))


local my_stats = db:first_cols("SELECT my_stats(col1, col2) FROM test")
print("my_stats:", my_stats)
