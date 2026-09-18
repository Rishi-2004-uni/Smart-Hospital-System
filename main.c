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
void calculateBilling(int i) {
    int s = selectedSpecialties[i] - 1;
    baseFeesList[i] = BASE_FEES[s];

    if (urgencyLevels[i] == 2) surchargesList[i] = baseFeesList[i] * 0.20;
    else if (urgencyLevels[i] == 3) surchargesList[i] = baseFeesList[i] * 0.50;
    else surchargesList[i] = 0.0;

    if (isAdmittedList[i] == 1) {
        wardCostsList[i] = daysAdmittedList[i] * WARD_RATES[selectedWards[i] - 1];
    } else {
        wardCostsList[i] = 0.0;
    }

    double grossTotal = baseFeesList[i] + surchargesList[i] + wardCostsList[i];

    if (patientAges[i] < 5 || patientAges[i] > 65) {
        discountsList[i] = grossTotal * 0.15;
    } else {
        discountsList[i] = 0.0;
    }

    finalPayables[i] = grossTotal - discountsList[i];

    waitingTimes[i] = specialtyQueueCount[s] * CONSULTATION_TIMES[s];
    specialtyQueueCount[s]++;
}

void printPatientBill(int i) {
    int s = selectedSpecialties[i] - 1;
    printf("\n============================================\n");
    printf("             PATIENT BILL & ADMISSION       \n");
    printf("============================================\n");
    printf("Patient ID         : PAT-%d\n", patientIDs[i]);
    printf("Name               : %s\n", patientNames[i]);
    printf("Specialty          : %s\n", SPECIALTY_NAMES[s]);
    if (isAdmittedList[i] == 1) {
        printf("Ward & Bed         : %s (Bed #%d)\n", WARD_NAMES[selectedWards[i] - 1], assignedBeds[i]);
    } else {
        printf("Ward               : None (Outpatient)\n");
    }
    printf("Base Fee           : LKR %.2f\n", baseFeesList[i]);
    printf("Emergency Surcharge: LKR %.2f\n", surchargesList[i]);
    printf("Ward Stay Cost     : LKR %.2f\n", wardCostsList[i]);
    printf("Age Subsidy Discount: LKR -%.2f\n", discountsList[i]);
    printf("--------------------------------------------\n");
    printf("Final Payable      : LKR %.2f\n", finalPayables[i]);
    printf("Estimated Wait Time: %.0f mins\n", waitingTimes[i]);
    printf("============================================\n");
}

void registerPatient() {
    if (patientCount >= MAX) {
        printf("Hospital Capacity Full!\n");
        return;
    }
    int i = patientCount;
    patientIDs[i] = 1001 + i;

    printf("\n--- Patient Registration (PAT-%d) ---\n", patientIDs[i]);
    printf("Enter Patient Name: ");
    scanf(" %[^\n]", patientNames[i]);
    printf("Enter Patient Age: ");
    scanf("%d", &patientAges[i]);
    printf("Enter Urgency Level (1-Normal, 2-Urgent, 3-Critical): ");
    scanf("%d", &urgencyLevels[i]);

    printf("Select Specialty (1-OPD, 2-Paediatrics, 3-Cardiology, 4-Neurology): ");
    scanf("%d", &selectedSpecialties[i]);

    printf("Admit to Ward? (1-Yes, 0-No): ");
    scanf("%d", &isAdmittedList[i]);

    if (isAdmittedList[i] == 1) {
        printf("Select Ward (1-General, 2-Paediatric, 3-Surgical, 4-ICU): ");
        scanf("%d", &selectedWards[i]);
        printf("Enter Days Admitted: ");
        scanf("%d", &daysAdmittedList[i]);

        int w = selectedWards[i] - 1;
        assignedBeds[i] = allocateBed(w);

        if (assignedBeds[i] == -1) {
            printf("Selected Ward is Full! Patient treated as Outpatient.\n");
            isAdmittedList[i] = 0;
            selectedWards[i] = 0;
            daysAdmittedList[i] = 0;
        }
    }

    calculateBilling(i);
    printPatientBill(i);

    patientCount++;
}

void displayPriorityQueue() {
    if (patientCount == 0) {
        printf("\nNo registered patients available!\n");
        return;
    }

    int indices[MAX];
    for (int i = 0; i < patientCount; i++) indices[i] = i;

    for (int i = 0; i < patientCount - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < patientCount; j++) {
            if (urgencyLevels[indices[j]] > urgencyLevels[indices[maxIdx]]) {
                maxIdx = j;
            }
        }
        int temp = indices[i];
        indices[i] = indices[maxIdx];
        indices[maxIdx] = temp;
    }

    printf("\n============================================\n");
    printf("          TRIAGE PRIORITY QUEUE             \n");
    printf("============================================\n");
    for (int k = 0; k < patientCount; k++) {
        int idx = indices[k];
        printf("ID: PAT-%d | Name: %-15s | Urgency: Level %d | Bill: LKR %.2f\n",
               patientIDs[idx], patientNames[idx], urgencyLevels[idx], finalPayables[idx]);
    }
    printf("============================================\n");
}

void generateAnalytics() {
    if (patientCount == 0) {
        printf("\nNo data available to generate analytics!\n");
        return;
    }

    double totalRevenue = 0, totalDiscounts = 0;
    int maxIdx = 0;

    for (int i = 0; i < patientCount; i++) {
        totalRevenue += finalPayables[i];
        totalDiscounts += discountsList[i];
        if (finalPayables[i] > finalPayables[maxIdx]) {
            maxIdx = i;
        }
    }

    printf("\n============================================\n");
    printf("          HOSPITAL ANALYTICS REPORT         \n");
    printf("============================================\n");
    printf("1. Total Patients Registered: %d\n", patientCount);
    printf("2. Total Revenue Earned     : LKR %.2f\n", totalRevenue);
    printf("3. Total Discounts Given    : LKR %.2f\n", totalDiscounts);

    printf("\n4. Bed Occupancy Rates:\n");
    for (int w = 0; w < 4; w++) {
        int occupied = 0;
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        double percentage = ((double)occupied / WARD_CAPACITIES[w]) * 100.0;
        printf("   - %-15s: %d/%d (%.1f%%)\n", WARD_NAMES[w], occupied, WARD_CAPACITIES[w], percentage);
    }

    printf("\n5. Highest Paying Patient:\n");
    printf("   - PAT-%d (%s) : LKR %.2f\n", patientIDs[maxIdx], patientNames[maxIdx], finalPayables[maxIdx]);
    printf("============================================\n");
}

int allocateBed(int wardIdx);
void calculateBilling(int i);
void printPatientBill(int i);
void registerPatient();
void displayPriorityQueue();
void generateAnalytics();

int main() {
    int choice;

    do {
        printf("\n============================================\n");
        printf("=== Smart Hospital Patient & Resource Allocation System ===\n");
        printf("============================================\n");
        printf("1. Register Patient & Generate Bill\n");
        printf("2. View Emergency Priority Queue\n");
        printf("3. View Analytics & Performance Report\n");
        printf("4. Exit\n");
        printf("Enter Choice (1-4): ");
        scanf("%d", &choice);

        if (choice == 1) registerPatient();
        else if (choice == 2) displayPriorityQueue();
        else if (choice == 3) generateAnalytics();
        else if (choice == 4) {
            printf("Exiting program...\n");
        } else {
            printf("Invalid choice! Try again.\n");
        }
    } while (choice != 4);

    return 0;
}


