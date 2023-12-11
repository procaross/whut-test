#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct Track {
  int track;
  bool isVisited;
};

int trackNum = 8;
vector<Track> tracks(trackNum);

int totalTrack = 0;

void initTracks();

void printSeparator(char separatorChar = '-', int length = 50) {
  std::cout << std::string(length, separatorChar) << std::endl;
}

/* First come first served */
void FCFS(int start) {
  initTracks();
  totalTrack = 0;
  int prev = start;
  for (int i = 0; i < trackNum; i++) {
    if (tracks[i].track == start) {
      tracks[i].isVisited = true;
      prev = start;
    } else {
      totalTrack += abs(tracks[i].track - prev);
      tracks[i].isVisited = true;
      prev = tracks[i].track;
    }
  }

  cout << "Track order: " << start << " → ";
  for (const auto& track : tracks) {
    cout << track.track << " → ";
  }
  cout << endl;

  cout << "Total tracks traversed: " << totalTrack << endl;
  cout << endl;
}

/* Shortest seek time first */
int closest(int now);
void SSTF(int start) {
  initTracks();

  totalTrack = 0;
  for (int i = 0; i < trackNum; i++) {
    if (i != 0) {
      totalTrack +=
          abs(tracks[closest(tracks[i - 1].track)].track - tracks[i - 1].track);
    }
    tracks[closest(tracks[i - 1].track)].isVisited = true;
  }

  cout << "Track order: " << start << " → ";
  for (const auto& track : tracks) {
    cout << track.track << " → ";
  }
  cout << endl;

  cout << "Total tracks traversed: " << totalTrack << endl << endl;
}

int closest(int now) {
  int min = 0x7FFFFFFF;
  int flag = -1;
  for (int i = 0; i < trackNum; i++) {
    if (!tracks[i].isVisited && abs(tracks[i].track - now) < min) {
      min = abs(tracks[i].track - now);
      flag = i;
    }
  }
  return flag;
}

/* Elevator algorithm */
void SCAN(int start, int direction) {
  initTracks();

  sort(tracks.begin(), tracks.end(),
       [](const Track& a, const Track& b) { return a.track < b.track; });

  totalTrack = 0;

  if (start <= tracks[0].track) {
    totalTrack = tracks[trackNum - 1].track - start;

    cout << "Track order: " << start << " → ";
    for (const auto& track : tracks) {
      cout << track.track << " → ";
    }
    cout << endl;
  }

  else if (start >= tracks[trackNum - 1].track) {
    totalTrack = start - tracks[0].track;

    cout << "Track order: " << start << " → ";
    for (auto it = tracks.rbegin(); it != tracks.rend(); ++it) {
      cout << it->track << " → ";
    }
    cout << endl;
  }

  else {
    vector<int> tempVector;
    cout << "Track order: " << start << " → ";
    int flag = 0;
    for (int i = 0; i < trackNum; i++) {
      if (tracks[i].track <= start) {
        tempVector.push_back(tracks[i].track);
        flag = i;
      }
    }

    if (direction == 0) {
      totalTrack = start - tracks[0].track + tracks[trackNum - 1].track -
                   tracks[0].track;

      for (const auto& track : tempVector) {
        cout << track << " → ";
      }
      for (int i = flag + 1; i < trackNum - 1; i++) {
        cout << tracks[i].track << " → ";
      }
      cout << tracks[trackNum - 1].track << endl;
    }

    else {
      totalTrack = tracks[trackNum - 1].track - tracks[0].track +
                   tracks[trackNum - 1].track - start;

      for (int i = flag + 1; i < trackNum; i++) {
        cout << tracks[i].track << " → ";
      }
      for (auto it = tempVector.rbegin(); it != tempVector.rend(); ++it) {
        cout << *it;
        if (it + 1 != tempVector.rend()) {
          cout << " → ";
        }
      }
      cout << endl;
    }
  }

  cout << "Total tracks traversed: " << totalTrack << endl << endl;

  printSeparator('-');
}

void initTracks() {
  for (int i = 0; i < trackNum; i++) {
    tracks[i].isVisited = false;
  }
}

int main() {
  printSeparator('-');

  cout << "Enter track access sequence:";
  for (int i = 0; i < trackNum; i++) {
    cin >> tracks[i].track;
  }
  cout << endl;

  printSeparator('-');

  while (true) {
    int choose, start, direction;

    cout << "Enter initial head position:";
    cin >> start;

    printSeparator('-');

    cout << "1: First come first served\n";
    cout << "2: Shortest seek time first\n";
    cout << "3: Elevator algorithm\n";
    cout << "Other: Quit\n";

    printSeparator('-');

    cout << "Enter your algorithm choice:";
    cin >> choose;

    printSeparator('-');

    if (choose != 1 && choose != 2 && choose != 3) break;

    switch (choose) {
      case 1:
        FCFS(start);
        break;

      case 2:
        SSTF(start);
        break;

      case 3:
        cout << "0: Move left\nOther: Move right\n";
        cout << "Enter head movement direction:";
        cin >> direction;
        printSeparator('-');
        SCAN(start, direction);
        break;

      default:
        break;
    }
  }
  return 0;
}
