#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

int main(void){
	
	int num_machines = 4;
    int flux_of_pieces = 4;
    int num_pieces = 1000;
    int storage_a1 = 0;
    int inventory = 0;
    int num_pieces_passed[4] = {5, 5, 10, 100};
    int notification_send = 1, notification_finish = 0;
    int i;

    int fd[flux_of_pieces][2];
    int fd_notification[flux_of_pieces][2];
    
    for(i = 0; i < flux_of_pieces; i++){
        if(pipe(fd[i]) == -1){
			perror("Pipe failed");
            return 1;
        }
    }
    
    for(i = 0; i < flux_of_pieces; i++){
        if(pipe(fd_notification[i]) == -1){
			perror("Pipe failed");
            return 1;
        }
    }
    
    printf("Number of pieces to produce: %d\n", num_pieces);
    
	pid_t pid = fork();

	if(pid == -1){
		perror("fork failed");
		return 1;
	}
	
	if(pid == 0){
		
		close(fd[0][0]);
		close(fd_notification[0][0]);
		
		while(num_pieces != 0){
			write(fd[0][1], &num_pieces_passed[0], sizeof(num_pieces_passed[0]));
			num_pieces -= 5;
			write(fd_notification[0][1], &notification_send, sizeof(notification_send));
			printf("%d pieces were send from machine M1 to machine M2\n", num_pieces_passed[0]);
		}
		
		write(fd_notification[0][1], &notification_finish, sizeof(notification_finish));
		
		close(fd[0][1]);
		close(fd_notification[0][1]);
		
		exit(0);
	}
	
	for(i = 1; i < num_machines; i++){
		pid_t machine = fork();

		if(machine == -1){
			perror("fork failed");
			return 1;
		}
		
		if(machine == 0){
		
			close(fd[i][0]);
			close(fd_notification[i][0]);
			
			int pieces_received, notification_received, pieces_to_send = 0;
			
			read(fd_notification[i-1][0], &notification_received, sizeof(int));
			
			while(notification_received != 0){
				if(notification_received == 1){
					read(fd[i-1][0], &pieces_received, sizeof(int));
					pieces_to_send += pieces_received;
					if(pieces_to_send == num_pieces_passed[i]){
						write(fd[i][1], &num_pieces_passed[i], sizeof(num_pieces_passed[i]));
						write(fd_notification[i][1], &notification_send, sizeof(notification_send));
						if(i < num_machines - 1){
							printf("%d pieces were send from machine M%d to machine M%d\n", num_pieces_passed[i], i+1, i+2);
						}
						else{
							printf("%d pieces were send from machine M%d to storage A1 and inventory\n", num_pieces_passed[i], i+1);
						}
						
						pieces_to_send = 0;
					}
				}
				read(fd_notification[i-1][0], &notification_received, sizeof(int));
			}
			
			write(fd_notification[i][1], &notification_finish, sizeof(notification_finish));
			
			close(fd[i][1]);
			close(fd_notification[i][1]);
			
			exit(0);
		}
	}
	
	// parent
	
	int pieces_received, notification_received;
	
	read(fd_notification[3][0], &notification_received, sizeof(int));
	
	while(notification_received != 0){
		if(notification_received == 1){
			read(fd[3][0], &pieces_received, sizeof(int));
			if(pieces_received == num_pieces_passed[3]){
				storage_a1 += pieces_received;
				inventory += pieces_received;
			}
		}
		read(fd_notification[3][0], &notification_received, sizeof(int));
	}
	
	close(fd[3][1]);
	close(fd_notification[3][1]);
	
	printf("The number of pieces in storage A1 is %d, and the number of produced parts in the inventory is %d\n", storage_a1, inventory);
	
	return 0;
}
