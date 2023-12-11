#include <iostream>
#include <string>
using namespace std;

int processNum = 0;
int totalRAM = 0;

int partitionNum = 0;
struct RAM {
  int processNo;
  int start = 0;
  int length = 0;
  int end = 0;
  int status = 0;
};
RAM ram[100];

int freeNum = 0;
struct FREERAM {
  int freePartNo = -1;
  int freeLength = -1;
  int freeStart = -1;
};
FREERAM freePart[100];

int reqNum = 0;
struct REQUEST {
  string reqProcessNo;
  int reqLength;
  int reqed = 0;
};
REQUEST request[100];

struct RamNode {
  int partNo;
  string processNo;
  int start = 0;
  int length = 0;
  int end = 0;
  int status = 0;
  RamNode *next;
};
typedef RamNode *ramNode;
ramNode head = NULL, tail = NULL, p, q;

void printPartitionTable();

void initialize() {
  cout << "*******Initialization******" << endl;
  cout << "Please enter the total memory size: ";
  cin >> totalRAM;
  cout << "Please enter the number of processes: ";
  cin >> processNum;
  cout << "Please enter the memory space required for processes 1 to "
       << processNum << ": ";
  for (int i = 0; i < processNum; i++) {
    int tempRam = 0;
    cin >> tempRam;

    if (i == 0 && tempRam < totalRAM) {
      p = new RamNode;
      p->partNo = i;
      p->processNo = to_string(i + 1);
      p->start = 0;
      p->length = tempRam;
      p->end = p->start + p->length - 1;
      p->status = 1;
      p->next = NULL;

      partitionNum++;

      if (head == NULL) {
        head = p;
        q = p;
      } else {
        q->next = p;
        q = p;
      }
    } else if (tempRam + q->end < totalRAM) {
      p = new RamNode;
      p->partNo = i;
      p->processNo = to_string(i + 1);
      p->start = q->end + 1;
      p->length = tempRam;
      p->end = p->start + p->length - 1;
      p->status = 1;
      p->next = NULL;

      partitionNum++;

      if (head == NULL) {
        head = p;
        q = p;
      } else {
        q->next = p;
        q = p;
      }
    }

    else {
      request[reqNum].reqProcessNo = to_string(i + 1);
      request[reqNum].reqLength = tempRam;
      reqNum++;
    }
  }

  if (q->end < totalRAM - 1) {
    p = new RamNode;
    p->partNo = q->partNo + 1;
    p->start = q->end + 1;
    p->end = totalRAM - 1;
    p->status = 0;
    p->next = NULL;
    p->length = totalRAM - p->start;
    if (head == NULL) {
      head = p;
      q = p;
    } else {
      q->next = p;
      q = p;
    }
  }

  tail = p;
}

void initializePartitionNo() {
  int i = 0;
  for (p = head; p != NULL; p = p->next) {
    p->partNo = i;
    i++;
  }
}

void insert(ramNode &p, int i) {
  p->processNo = request[i].reqProcessNo;
  p->status = 1;

  if (request[i].reqLength == p->length) {
  }

  else {
    q = new RamNode;
    q->end = p->end;
    p->end = p->start + request[i].reqLength - 1;
    p->length = request[i].reqLength;

    q->partNo = p->partNo + 1;
    q->start = p->end + 1;
    q->length = q->end - q->start + 1;
    q->status = 0;

    q->next = p->next;
    p->next = q;
  }

  request[i].reqed = 1;
}

void firstFit(int &i) {
  for (i; i < reqNum; i++) {
    for (p = head; p != NULL; p = p->next) {
      if (request[i].reqLength <= p->length && p->status == 0) {
        insert(p, i);
        break;
      }
    }

    if (i == 0 && p == NULL) {
      return;
    } else if (p == NULL) {
      return;
    }
  }
}

void refreshRequest(int i) {
  int temp = reqNum;
  reqNum = reqNum - i;
  for (int j = 0; j < temp; i++, j++) {
    request[j].reqProcessNo = request[i].reqProcessNo;
    request[j].reqLength = request[i].reqLength;
    request[j].reqed = request[i].reqed;
  }
}

void firstFitAlgorithm() {
  cout << "Current request table information:" << endl;
  cout << "Order\tProcess Name\tRequest Length" << endl;
  for (int i = 0; i < reqNum; i++) {
    cout << i + 1 << "\t" << request[i].reqProcessNo << "\t"
         << request[i].reqLength << endl;
  }
  int i = 0;
  firstFit(i);

  if (i == 0) {
    cout << "Insufficient space under this strategy, no allocations were made!"
         << endl;
    return;
  }

  else if (i == reqNum) {
    cout << "All processes in the request table have been allocated!" << endl;
  }

  else {
    cout << "Partial allocations have been made. Remaining unallocated "
            "processes:"
         << endl;
    cout << "\tProcess Name\tRequest Length" << endl;
    for (int i = 0; i < reqNum; i++) {
      if (request[i].reqed == 0) {
        cout << "\t" << request[i].reqProcessNo << "\t" << request[i].reqLength
             << endl;
      }
    }
  }
  cout << "Memory usage after allocation:" << endl;
  printPartitionTable();
  initializePartitionNo();

  refreshRequest(i);
  return;
}

void swap(int &a, int &b) {
  int temp;
  temp = a;
  a = b;
  b = temp;
}

void findFreePartitions() {
  initializePartitionNo();

  for (int i = 0; i <= freeNum; i++) {
    freePart[i].freePartNo = -1;
    freePart[i].freeStart = -1;
    freePart[i].freeLength = -1;
  }
  freeNum = 0;

  for (p = head; p != NULL; p = p->next) {
    if (p->status == 0) {
      freePart[freeNum].freePartNo = p->partNo;
      freePart[freeNum].freeStart = p->start;
      freePart[freeNum].freeLength = p->length;
      freeNum++;
    }
  }
}

void initializeBestFit() {
  findFreePartitions();

  for (int i = 0; i < freeNum - 1; i++) {
    for (int j = 0; j < freeNum - i - 1; j++) {
      if (freePart[j].freeLength > freePart[j + 1].freeLength) {
        swap(freePart[j].freePartNo, freePart[j + 1].freePartNo);
        swap(freePart[j].freeStart, freePart[j + 1].freeStart);
        swap(freePart[j].freeLength, freePart[j + 1].freeLength);
      }
    }
  }
}

int findInsertPartitionNo(int i) {
  int partNo = -1;
  for (int j = 0; j < freeNum; j++) {
    if (request[i].reqLength <= freePart[j].freeLength) {
      partNo = freePart[j].freePartNo;

      return partNo;
    }
  }
  return -1;
}

void bestFit() {
  cout << "Current request table information:" << endl;
  cout << "Order\tProcess Name\tRequest Length" << endl;
  for (int i = 0; i < reqNum; i++) {
    cout << i + 1 << "\t" << request[i].reqProcessNo << "\t"
         << request[i].reqLength << endl;
  }

  initializeBestFit();
  int i = 0;
  for (i; i < reqNum; i++) {
    int partNo = -1;
    partNo = findInsertPartitionNo(i);

    if (i == 0 && partNo == -1) {
      cout
          << "Insufficient space under this strategy, no allocations were made!"
          << endl;
      return;
    } else if (partNo == -1) {
      cout << "Only partial processes have been allocated! Remaining "
              "unallocated processes:"
           << endl;
      cout << "\tProcess Name\tRequest Length" << endl;
      for (int j = 0; j < reqNum; j++) {
        if (request[j].reqed == 0) {
          cout << "\t" << request[j].reqProcessNo << "\t"
               << request[j].reqLength << endl;
        }
      }
      break;
    }

    for (p = head; p != NULL; p = p->next) {
      if (p->partNo == partNo) {
        insert(p, i);
      }
    }

    initializeBestFit();
  }
  if (i == reqNum)
    cout << "All processes in the request table have been allocated!" << endl;
  printPartitionTable();

  refreshRequest(i);
}

void initializeWorstFit() {
  findFreePartitions();

  for (int i = 0; i < freeNum - 1; i++) {
    for (int j = 0; j < freeNum - i - 1; j++) {
      if (freePart[j].freeLength < freePart[j + 1].freeLength) {
        swap(freePart[j].freePartNo, freePart[j + 1].freePartNo);
        swap(freePart[j].freeStart, freePart[j + 1].freeStart);
        swap(freePart[j].freeLength, freePart[j + 1].freeLength);
      }
    }
  }
}

void worstFit() {
  cout << "Current request table information:" << endl;
  cout << "Order\tProcess Name\tRequest Length" << endl;
  for (int i = 0; i < reqNum; i++) {
    cout << i + 1 << "\t" << request[i].reqProcessNo << "\t"
         << request[i].reqLength << endl;
  }

  initializeWorstFit();
  int i = 0;
  for (i; i < reqNum; i++) {
    int partNo = -1;
    partNo = findInsertPartitionNo(i);

    if (i == 0 && partNo == -1) {
      cout
          << "Insufficient space under this strategy, no allocations were made!"
          << endl;
      return;
    } else if (partNo == -1) {
      cout << "Only partial processes have been allocated! Remaining "
              "unallocated processes:"
           << endl;
      cout << "\tProcess Name\tRequest Length" << endl;
      for (int j = 0; j < reqNum; j++) {
        if (request[j].reqed == 0) {
          cout << "\t" << request[j].reqProcessNo << "\t"
               << request[j].reqLength << endl;
        }
      }
      break;
    }

    for (p = head; p != NULL; p = p->next) {
      if (p->partNo == partNo) {
        insert(p, i);
      }
    }

    initializeWorstFit();
  }
  if (i == reqNum)
    cout << "All processes in the request table have been allocated!" << endl;
  printPartitionTable();

  refreshRequest(i);
}

void allocatePartition() {
  if (reqNum == 0) {
    cout << "There are currently no processes with unallocated memory!" << endl
         << endl;
    return;
  }
  cout << "1. First Fit Algorithm" << endl;
  cout << "2. Best Fit Algorithm" << endl;
  cout << "3. Worst Fit Algorithm" << endl;
  cout << "Please select an allocation strategy: ";
  int choice;
  cin >> choice;

  switch (choice) {
    case 1:
      firstFitAlgorithm();
      break;

    case 2:
      bestFit();
      break;

    case 3:
      worstFit();
      break;

    default:
      break;
  }
}

void printPartitionTable() {
  cout << "P No.\tStart\tLength\tEnd\tStatus\tOccupied Process No." << endl;
  int i = 0;
  for (p = head; p != NULL; p = p->next) {
    cout << " " << p->partNo << "\t" << p->start << "\t" << p->length << "\t"
         << p->end << "\t";
    if (p->status == 1)
      cout << "Occupied\t" << p->processNo << endl;
    else
      cout << "Free" << endl;
    i++;
  }
  cout << endl;
}

void releasePartition() {
  cout << "Please enter the partition number to be released: ";
  int freePart;
  cin >> freePart;
  p = head;
  for (int i = 0; i <= freePart; i++) {
    if (i != freePart) p = p->next;
  }
  if (p->status == 1) {
    p->status = 0;
    p->processNo = "";
    cout << "Partition " << freePart << " released successfully!" << endl;

    for (p = head; p != NULL;) {
      if (p->status == 0 && p->next != NULL) {
        if (p->next->status == 0) {
          p->end = p->next->end;
          p->length = p->length + p->next->length;

          q = p->next;
          p->next = q->next;
          delete q;
        } else {
          p = p->next;
        }
      } else {
        p = p->next;
      }
    }

    initializePartitionNo();
  } else {
    cout << "The current partition is free, no need to release it!" << endl;
  }
  printPartitionTable();
  cout << endl;
}

void addProcessToRequest() {
  cout << "Please enter the process number and request length: ";
  if (1) {
    cin >> request[reqNum].reqProcessNo >> request[reqNum].reqLength;
    reqNum++;
    cout << "Added successfully!" << endl;
  }
  cout << endl;
}

void showUnAllocated() {
  cout << "Unallocated processes:" << endl;
  cout << "Process Name\tRequest Length" << endl;
  for (int j = 0; j < reqNum; j++) {
    if (request[j].reqed == 0) {
      cout << request[j].reqProcessNo << "\t"
           << "\t" << request[j].reqLength << endl;
    }
  }
  cout << endl;
}

int main() {
  initialize();
  while (1) {
    cout << "********Make your choice:)********" << endl;
    cout << "1. Display partition status" << endl;
    cout << "2. Release partition" << endl;
    cout << "3. Allocate partition" << endl;
    cout << "4. Add a process" << endl;
    cout << "5. View unallocated processes" << endl;

    cout << "Please enter your option: ";
    int choice;
    cin >> choice;

    switch (choice) {
      case 1:
        printPartitionTable();
        break;

      case 2:
        releasePartition();
        break;

      case 3:
        allocatePartition();
        break;

      case 4:
        addProcessToRequest();
        break;

      case 5:
        showUnAllocated();
        break;

      default:
        break;
    }
  }
  return 0;
}
