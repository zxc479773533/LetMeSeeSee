/*
 * Example for using LetMeSeeSee
 * 
 * Remote system monitor
 * 
 * Crated at 2019-02-22 by zxcpyp
 */

// Add the LetMeSeeSee header
#include <LetMeSeeSee/storager.h>

#include <climits>
#include "lib/sys/cmdparser.h"
#include "lib/sys/time.h"
#include "lib/io/file.h"

std::string datadir;  // Savedata directory
pylib::Clock timer;   // Global clock

// ----- Function Prototypes for use -----
void print_usage();
std::string get_process_info();
std::string get_cpu_info();
std::string get_memory_info();
// ---------------------------------------


// ----- Here add the store function -----
Store(Process_Info, Process Information Table) {
  std::string SaveFile = datadir + "/process_info.csv";
  pylib::WriteFile(SaveFile, get_process_info());
  return SaveFile;
}

Store(CPU_Info, CPU Information) {
  std::string SaveFile = datadir + "/cpu_info.txt";
  pylib::WriteFile(SaveFile, get_cpu_info());
  return SaveFile;
}

Store(Memory_Info, Memory Use Information) {
  std::string SaveFile = datadir + "/memory_info.txt";
  pylib::WriteFile(SaveFile, get_memory_info());
  return SaveFile;
}
// ---------------------------------------


// -------------- Your Code --------------

int main(int argc, char **argv) {
  // Use the storager
  lmss::Storager storager;

  // Handle arguments
  std::string srcfile, logfile;
  int local = 0;
  int freq = 1;
  pylib::CmdParser parser(argc, argv);
  while (parser.NowPos() < parser.OperateNum()) {
    std::queue<std::string> arg_list;
    std::string op = parser.Next(arg_list);
    if (op == "source")
      srcfile = arg_list.front();
    else if (op == "datadir")
      datadir = arg_list.front();
    else if (op == "logfile")
      logfile = arg_list.front();
    else if (op == "local") {
      local = 1;
      freq = std::stoi(arg_list.front());
    }
    else {
      print_usage();
      exit(0);
    }
  }
  if (srcfile.empty() || datadir.empty()) {
    print_usage();
    exit(0);
  }
  if (!logfile.empty()) {
    storager.SetLogFile(logfile);
  }

  // Specify the source directory
  storager.ScanSourceCode(srcfile);
  // Start the server
  storager.ListenAndServe("0.0.0.0", 1234);

  pylib::Clock timer;
  while (true) {
    if (local == 1) {
      pylib::WriteFile(datadir + "/process_info.csv", get_process_info());
      pylib::WriteFile(datadir + "/cpu_info.txt", get_cpu_info());
      pylib::WriteFile(datadir + "/memory_info.txt", get_memory_info());
    }
    sleep(freq);
  }
}

void print_usage() {
  pylib::println("Usage: ./RemoteSM [Options]");
  pylib::println("Options:");
  pylib::println("  --source  <dir> : Specify source directory [Required]");
  pylib::println("  --datadir <dir> : Specify the savedata directory [Required]");
  pylib::println("  --logfile <dir> : Specify the log output directory");
  pylib::println("  --local  <freq> : Store data by frequency");
}

std::string get_process_info() {
  std::string process_data = "PID,Name,PPID,State,Priority,Memory Use\n";
  pylib::OpenDir("/proc").Traverse([&process_data](pylib::FileInfo &&info) {
    // Judge process file
    if (info.Name()[0] >= '0' && info.Name()[0] <= '9') {
      auto stat = pylib::split(pylib::OpenFile("/proc/" + info.Name() + "/stat", false, O_RDONLY).Read(INT_MAX), " ");
      if (!stat[0].empty()) {
        std::string line = stat[0] + ","
                         + stat[1].substr(1, stat[1].size() - 2) + ","
                         + stat[3] + ","
                         + stat[2] + ","
                         + stat[17] + ",";
        auto buf = new char[32];
        if (std::stoi(stat[23]) > 1024) {
          double MemUse = std::stoi(stat[23]) / 1024.0;
          sprintf(buf, "%.1f MB", MemUse);
        }
        else if (std::stoi(stat[23]) > 0) {
          sprintf(buf, "%d.0 KB", std::stoi(stat[23]));
        }
        else {
          sprintf(buf, "0");
        }
        line += std::string(buf) + "\n";
        delete[] buf;
        process_data += line;
      }
    }
  });
  return process_data;
}

std::string get_cpu_info() {
  std::string cpu_data = "";
  static int flag = 0;
  static double cpu_ratio = 0;
  int user, nice, system, idle, iowait, irq, softirq;
  double total, idle_diff, total_diff;
  static double old_idle, old_total;
  auto buf = new char[64];
  timer.Refresh();
  sprintf(buf, "Time: %d-%d-%d %02d:%02d:%02d\n",
   timer.getyear(), timer.getmonth(), timer.getday(), timer.gethour(), timer.getminute(), timer.getsecond());
  cpu_data += std::string(buf);
  // Get CPU information
  auto cpu = pylib::split(pylib::split(pylib::OpenFile("/proc/cpuinfo", false, O_RDONLY).Read(INT_MAX), "\n\n")[0], "\n");
  for (int i = 0; i < cpu.size(); i++) {
    if (cpu[i].find("model name") != std::string::npos)
      cpu_data += "        CPU Name: " + cpu[i].substr(cpu[i].rfind(":") + 2) + "\n";
    else if (cpu[i].find("cpu MHz") != std::string::npos)
      cpu_data += "         CPU MHz: " + cpu[i].substr(cpu[i].rfind(":") + 2) + "\n";
    else if (cpu[i].find("cache size") != std::string::npos)
      cpu_data += "      Cache size: " + cpu[i].substr(cpu[i].rfind(":") + 2) + "\n";
    else if (cpu[i].find("cpu cores") != std::string::npos)
      cpu_data += "       CPU Cores: " + cpu[i].substr(cpu[i].rfind(":") + 2) + "\n";
    else if (cpu[i].find("address sizes") != std::string::npos)
      cpu_data += "Addressing digit: " + cpu[i].substr(cpu[i].rfind(":") + 2) + "\n";
  }
  // Get CPU use ratio
  auto cpuUse = pylib::split(pylib::OpenFile("/proc/stat", false, O_RDONLY).Read(INT_MAX), "\n");
  auto cpu_use_data = pylib::split(cpuUse[0], " ");
  user = std::stoi(cpu_use_data[2]);
  nice = std::stoi(cpu_use_data[3]);
  system = std::stoi(cpu_use_data[4]);
  idle = std::stoi(cpu_use_data[5]);
  iowait = std::stoi(cpu_use_data[6]);
  irq = std::stoi(cpu_use_data[7]);
  softirq = std::stoi(cpu_use_data[8]);
  // First time
  if (flag == 0) {
    flag = 1;
    old_idle = idle / cpuUse.size();
    old_total = (user + nice + system + idle + iowait + irq + softirq) / cpuUse.size();
    cpu_ratio = 0;
  }
  else {
    total = (user + nice + system + idle + iowait + irq + softirq) / cpuUse.size();
    total_diff = total - old_total;
    idle_diff = idle / cpuUse.size() - old_idle;
    cpu_ratio = 100 * (total_diff - idle_diff) / total_diff;
    total = old_total;
    idle = old_idle;
  }
  sprintf(buf, "         CPU Use: %.2f%%\n", cpu_ratio);
  cpu_data += std::string(buf) + "\n";
  delete[] buf;
  return cpu_data;
}

std::string get_memory_info() {
  std::string mem_data = "";
  int mem_total, mem_free, mem_use;
  int swap_total, swap_free, swap_use;
  auto buf = new char[64];
  timer.Refresh();
  sprintf(buf, "Time: %d-%d-%d %02d:%02d:%02d\n",
   timer.getyear(), timer.getmonth(), timer.getday(), timer.gethour(), timer.getminute(), timer.getsecond());
  mem_data += std::string(buf);
  auto MemUse = pylib::split(pylib::OpenFile("/proc/meminfo", false, O_RDONLY).Read(INT_MAX), "\n");
  for (int i = 0; i < MemUse.size(); i++) {
    if (MemUse[i].find("MemTotal") != std::string::npos) {
      auto tmp = pylib::split(MemUse[i], " ");
      mem_total = std::stoi(tmp[tmp.size() - 2]);
    }
    if (MemUse[i].find("MemAvailable") != std::string::npos) {
      auto tmp = pylib::split(MemUse[i], " ");
      mem_free = std::stoi(tmp[tmp.size() - 2]);
    }
    if (MemUse[i].find("SwapTotal") != std::string::npos) {
      auto tmp = pylib::split(MemUse[i], " ");
      swap_total = std::stoi(tmp[tmp.size() - 2]);
    }
    if (MemUse[i].find("SwapFree") != std::string::npos) {
      auto tmp = pylib::split(MemUse[i], " ");
      swap_free = std::stoi(tmp[tmp.size() - 2]);
    }
  }
  mem_use = mem_total - mem_free;
  swap_use = swap_total - swap_free;
  sprintf(buf, "  Memory Use: %.1f MB\n", mem_use / 1024.0);
  mem_data += std::string(buf);
  sprintf(buf, "Memory Total: %.1f MB\n", mem_total / 1024.0);
  mem_data += std::string(buf);
  sprintf(buf, "  Memory Use: %.2f%%\n\n", 100 * (double)mem_use / mem_total);
  mem_data += std::string(buf);
  sprintf(buf, "    Swap Use: %.1f MB\n", swap_use / 1024.0);
  mem_data += std::string(buf);
  sprintf(buf, "  Swap Total: %.1f MB\n", swap_total / 1024.0);
  mem_data += std::string(buf);
  sprintf(buf, "    Swap Use: %.2f%%\n", 100 * (double)swap_use / swap_total);
  mem_data += std::string(buf);
  delete[] buf;
  return mem_data;
}