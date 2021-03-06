
#include "ff/sql/mysql.hpp"
#include "ff/sql/mysql_types.h"
#include "ff/sql/table.h"

struct mymeta {
  constexpr static const char *table_name = "xxxyyy";
};

struct md {
  int a;
  int b;
  int c;
};

using namespace ff::mysql;
define_column(c1, column, uint64_t, "id");
define_column(xx, column, char_m<123>, "id");
typedef ff::sql::table<ff::sql::mysql<ff::sql::cppconn>, mymeta, c1, xx>
    mytable;

int main(int argc, char *argv[]) {

  ff::sql::mysql<ff::sql::cppconn> engine("tcp://127.0.0.1:3306", "root", "",
                                          "test");
  try {
    mytable::drop_table(&engine);
  } catch (...) {
    std::cout << "drop_table failed" << std::endl;
  }
  mytable::create_table(&engine);

  mytable::row_collection_type rows;

  mytable::row_collection_type::row_type t1;
  t1.set<c1, xx>(1, "hi");
  rows.push_back(std::move(t1));

  mytable::insert_or_replace_rows(&engine, rows);

  auto ret1 = mytable::select<c1, xx>(&engine).eval();
  std::cout << "size: " << ret1.size() << std::endl;
  for (size_t i = 0; i < ret1.size(); ++i) {
    std::cout << ret1[i].get<c1>() << ", " << ret1[i].get<xx>().data()
              << std::endl;
  }
  std::cout << "---------------" << std::endl;
  // mytable::select<c1, c2>(&engine).where(c1::eq(1)).eval();
  return 0;
}
