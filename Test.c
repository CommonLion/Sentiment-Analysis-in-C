#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The purpose of this exercise was to attempt a sentiment analysis in C. C is not the ideal language for doing this because
C does not handle strings easily. 
Procedure:
Clean the reviews (I had to clean out all information that was not relevant)
Import the reviews (I chose to put the reviews into a struct. During this process, I created another struct called summary, to store the related summary to the review.)
Create the "bag of words"(I took 100+ of the most common good and bad words and stored them into two different arrays.)
Populate the review structs(I ran the functions wordcount, sgoodwords, sbadwords, rgoodwords, and sbadwords to get most of the data needed. The other variables were imported from the csv)
Perform Naive Bayes(Naive Bayes was the method I attempted to use to determine whether the review was good or bad. However, it did not return consistent results. Eventually, I decided to use
the rating as the final determinate if the results were strange. )

 */

typedef int tag;
#define postive 0
#define negative 1

struct Summary {
	int Stag;
	char Summary[500];
	double PBad;
	double PGood;
	int GoodWord;
	int BadWord;
	int WordCount;
};

struct Review {
	int ID;
	char Review[1000];
	int Rating;
	int WordCount;
	int GoodWord;
	int BadWord;
	int Rtag;
	int Stag;
	struct Summary Summary;
	double PBad;
	double PGood;

};

void SummaryProbablities(struct Summary S) {}
char Good_Words[102][17] = { "amazing","awesome","beneficial","bright","brillant","clean","creative","cute","easy","essential","excellent","exceptional","fabulous"
,"fantastic","fine","free","fun","genius","glad","genuis","glad","good","great","happy","hearty","ideal","impressive","kind","like","love","lovely","nice","perfect"
,"plentiful","popular","powerful","powerful","pretty","quick","can recommend","refined","reliable","remarkable","rewarding","robust","satisfactory","satisfied"
,"simple","solid","special","stunning","super","superb","sweet","terrific","thanks","transformative","useful","vibrant","well","wellmade","what can i say"
,"wonderful","wondrous","worked","worked fine","worth","yup","works","soft","warm","unbeatable","unbelievable","what i need","use it","use this","best","strong"
,"improved","new","great","thick","better","accurate","unique","good","five stars","four stars","sound","helpful","expected","working","tight fit","handy","springy"
,"what can you say","5 stars","pleased","lightweight","affordable","acceptable","5 star"
};

char Bad_Words[110][15] = { "adequate","annoyance","annoying","average","avoid","awkward","bizarre","boring","zero","broken","bummer","busted","buzz","can't","cheap","cheaply"
,"crap","cumbersome","dirty","disappointed","digusted","displeased","does not","dull","flat","flimsy","garbage","iffy","joke","junk","noise","not bad","not good","ok"
,"stupid","toy","useless","vary","waste","you get what","yuck","yuk","pass","barely","sent it back","horrible","careful","issue","beware","break", "go build","poor","we get what"
,"lousy","clunky","died","issues","terrible","wtf","unfair","warning","weird", "not sturdy","different","sick","subsitutions","frustrating","worst","bare","bleh","stop working"
,"waste of money","shaky","low","glorified","prevent", "badly designed","dead","died","wiggly","defective","fluke","uninspiring","difficult","quit working","old","avoid","awful","problems"
,"alright"," 1 star"," 1 stars","2 stars","3 stars", "three stars", "two stars","complaint","miss wired","careful","weren't","didn't","wasn't","pain","tricky","decent","stiff","dont bother"
,"wobbly","didn't work","accident"
};

int SWordCount(struct Review Sum) {
	int WordCount = -1;
	for (int i = 0; Sum.Summary.Summary[i] != '\0'; i++) {
		if (Sum.Summary.Summary[i] == ' ')
			WordCount++;
	}
	return WordCount;
}

int SGoodWords(struct Review Sum) {
	int GoodWords = 0;
	for (int i = 0; i < 102; i++) {
		if (strstr(Sum.Summary.Summary, Good_Words[i]))
			GoodWords++;
	}	
	return GoodWords;
}

int SBadWords(struct Review Sum) {
	int BadWords = 0;
	for (int i = 0; i < 110; i++) {
		if (strstr(Sum.Summary.Summary, Bad_Words[i]))
			BadWords++;
	}
	return BadWords;
}

int RGoodWords(struct Review Rev) {
	int GoodWords = 0;
	for (int i = 0; i < 101; i++) {
		if (strstr(Rev.Review, Good_Words[i]))
			GoodWords++;
	}
	return GoodWords;
}

int RBadWords(struct Review Rev) {
	int BadWords = 0;
	for (int i = 0; i < 110; i++) {
		if (strstr(Rev.Review, Bad_Words[i]))
			BadWords++;
	}
	return BadWords;
}
	

void main() {

	char *filename = "Reviews.csv";
	FILE *fp;
	int N_Reviews = 246;

	fp = fopen(filename, "r");

	struct Review ReviewArray[246];

	for (int i = 0; i < N_Reviews; i++) {
		struct Review _i;
		fscanf(fp, "%d,%[^,],%[^,],%d", &ReviewArray[i].ID,&ReviewArray[i].Review,&ReviewArray[i].Summary.Summary,&ReviewArray[i].Rating );
	}

	int N_postiveSummaries = 0;
	int N_negativeSummaries = 0;
	int N_postiveReviews = 0;
	int N_negativeReviews = 0;
	

	for (int i = 0; i < N_Reviews; i++) {
		ReviewArray[i].Summary.WordCount = SWordCount(ReviewArray[i]);
		ReviewArray[i].Summary.GoodWord = SGoodWords(ReviewArray[i]);
		ReviewArray[i].Summary.BadWord = SBadWords(ReviewArray[i]);
		ReviewArray[i].Summary.PBad = ReviewArray[i].Summary.BadWord / (ReviewArray[i].Summary.BadWord + ReviewArray[i].Summary.GoodWord);
		ReviewArray[i].Summary.PGood = ReviewArray[i].Summary.GoodWord / (ReviewArray[i].Summary.BadWord + ReviewArray[i].Summary.GoodWord);
		if (ReviewArray[i].Summary.PBad > ReviewArray[i].Summary.PGood) {
			ReviewArray[i].Summary.Stag = negative;
			N_negativeSummaries++;
		}
		else {
			ReviewArray[i].Summary.Stag = postive;
			N_postiveSummaries++;
		}
		ReviewArray[i].BadWord = RBadWords(ReviewArray[i]);
		ReviewArray[i].GoodWord = RGoodWords(ReviewArray[i]);

		ReviewArray[i].PBad = ReviewArray[i].BadWord / (ReviewArray[i].BadWord + ReviewArray[i].GoodWord);
		ReviewArray[i].PGood = ReviewArray[i].GoodWord / (ReviewArray[i].BadWord + ReviewArray[i].GoodWord);
			if (ReviewArray[i].Rating <= 3 || ReviewArray[i].PBad > ReviewArray[i].PGood){
				ReviewArray[i].Rtag = negative;
			}else{ 
		ReviewArray[i].Rtag = postive;
		}
	}

	


	for (int i = 0; i < N_Reviews; i++) {
		printf(" %d %s \n", ReviewArray[i].ID, ReviewArray[i].Review);
	
	}
	/* This section was for demonstration purposes in class
	printf("\nLets test this program! \n\n");

	printf("Is this review good or bad?\n");
	printf("Positive = 0 Negative = 1\n");

	printf("%s\n", ReviewArray[2].Review);

	printf("Survey Says: %d \n\n", ReviewArray[2].Rtag);

	printf("Let's try another!\n");
	printf("Positive = 0 Negative = 1\n");

	printf("%s\n", ReviewArray[239].Review);
	printf("Survey Says: %d \n", ReviewArray[239].Rtag);
*/

	fclose(fp);
	

	
}