// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UArduinoComms.generated.h"

USTRUCT(BlueprintType)
struct FSplutData
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arduino")
    float xPos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arduino")
    float yPos;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arduino")
    float ballSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arduino")
    float ballSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arduino")
    bool splutReceived;
    
};

/**
 * 
 */
UCLASS(BlueprintType)
class ACETROIDS_4_10_API UUArduinoComms : public UObject
{
	GENERATED_BODY()
    char        m_buf[256];
    
    int32 serialport_init(FString serialport, int32 baud);
    int32 serialport_close(int32 fd);
    int32 serialport_writebyte( int32 fd, uint8_t b);
    int32 serialport_write(int32 fd, const char* str);
    int32 serialport_read_until(int32 fd, char* buf, char until, int32 buf_max,int32 timeout);
    FSplutData serialport_read_splut_if_any(int32 fd);
    int32 serialport_flush(int32 fd);
	
public:
	UFUNCTION(BlueprintCallable, Category="Arduino")
    int32 initSerial(FString portName, int32 baudRate);
	
    UFUNCTION(BlueprintCallable, Category="Arduino")
    FString readData(int32 fd);
    
    UFUNCTION(BlueprintCallable, Category="Arduino")
    FSplutData readSplut(int32 fd);
    
    UFUNCTION(BlueprintCallable, Category="Arduino")
    int32 closeSerial(int32 fd);
    
    UFUNCTION(BlueprintCallable, Category="Arduino")
    TArray<FString> getSerialPorts();
    
    UFUNCTION(BlueprintCallable, Category="Arduino")
    FString getSavedPath();
};
