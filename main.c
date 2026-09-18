#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

const int SPECIALTY_IDS[4] = {1, 2, 3, 4};
const char* SPECIALTY_NAMES[4] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const double BASE_FEES[4] = {1500.00, 2500.00, 4500.00, 5000.00};
const int CONSULTATION_TIMES[4] = {15, 20, 30, 30};
const int DAILY_PATIENT_CAPS[4] = {30, 20, 12, 10};

const int WARD_IDS[4] = {1, 2, 3, 4};
const char* WARD_NAMES[4] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"};
const double WARD_RATES[4] = {3000.00, 6000.00, 12000.00, 25000.00};
const int WARD_CAPACITIES[4] = {20, 10, 10, 5};

int bedOccupancy[4][20] = {0};
int specialtyQueueCount[4] = {0};

int patientIDs[MAX];
char patientNames[MAX][50];
int patientAges[MAX];
int urgencyLevels[MAX];
int selectedSpecialties[MAX];
int isAdmittedList[MAX];
int selectedWards[MAX];
int assignedBeds[MAX];
int daysAdmittedList[MAX];

double baseFeesList[MAX];
double surchargesList[MAX];
double wardCostsList[MAX];
double discountsList[MAX];
double finalPayables[MAX];
double waitingTimes[MAX];

int patientCount = 0;

int allocateBed(int wardIdx) {
    for (int b = 0; b < WARD_CAPACITIES[wardIdx]; b++) {
        if (bedOccupancy[wardIdx][b] == 0) {
            bedOccupancy[wardIdx][b] = 1;
            return b + 1;
        }
    }
    return -1;
}

int main() {
    printf("=== Smart Hospital Patient & Resource Allocation System ===\n");
    return 0;
}
