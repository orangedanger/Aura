# UE5 GAS游戏的开始

## Bug:

1.`属性菜单Bug`:在对Primary Effect 中属性初始化时用到了Max属性导致Client的属性菜单中数据显示为0(原因未知)

2.`Overlap血条显示Bug`：客户端时Max属性Broadcast值为0；（好像是顺序问题，我将Max属性放在Health和Mana之前即可显示正确)

3.多人游戏中客户端的血条没有显示

4.Projectile生成后不会自动销毁

5.关于造成伤害时UE_Log有报错说明: (在GetSetByCallerMagnitude中可以设置变量WarnIfNotFound 设置为false)



## 1.资产导入

```c++
//导入到Context里面
```

## 2.人物的创建

```c++
//新建Asset文件夹(存放资产)
//将资产全部转移进入到Asset文件夹中
//设置Blueprints文件夹(存放蓝图)
//创建角色新建主角文件夹角色名这里用Aura(存放角色蓝图)
//新建C++人物AuraCharacter
//可以加Camera(摄像机)和CameraBoom(摄像机臂)
//基于人物AuraCharacter创建BP_Aura蓝图
//设置蓝图资产

```



## 3.动画蓝图设置

## 4.增强输入和Controller

需要添加[ EnhancedInput](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/EnhancedInput)Module才能进行添加 UInputMappingContext

```c++
//绑定增强输出组件
check(Context);
UEnhancedInputLocalPlayerSubsystem* Subsytem =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
check(Subsytem);
Subsytem->AddMappingContext(Context, 0);

//鼠标样式
bShowMouseCursor = true;
DefaultMouseCursor = EMouseCursor::Default;

//鼠标控制和鼠标的显示
FInputModeGameAndUI InputDate;
InputDate.SetHideCursorDuringCapture(false);
InputDate.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
SetInputMode(InputDate);


void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//获取增强输入组件
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//增加绑定
	EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);



}

void AAuraPlayerController::Move(const FInputActionValue& InputValue)
{
	const FVector2D PlayerAxisVector = InputValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	//通过矩阵获取X,Y方向大小
	const FVector ForwordDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	APawn* ControllerPawn = GetPawn<APawn>();
	if (ControllerPawn)
	{
		ControllerPawn->AddMovementInput(ForwordDir, PlayerAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDir, PlayerAxisVector.X);
	}
}


```

## 5.Enemy的接口类以显示高亮(HighLight)

```c++
//EnemyInterface 目的降低耦合
//HighLight和UnHighLight函数


//放置后期盒(PostProcessVolume)
//开启 Infinite Extent(Unbound)  无限范围按钮
//Post Process Materials增加 发光材质
//后期处理材质(WeightedBlendables) 
//项目设置中(custom depth-Stencil Pass)Enabled with Stencil
//添加材质


	//显示材质
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);


```

## 6.游戏Ability类创建

```c++
由于Gameplay技能系统是一个插件，因此你需要先启用它才能使用。只需两步即可启用它：
在 编辑（Edit） -> 插件（Plugins） 窗口中启用Gameplay技能系统插件。

Build.cs 文件中加入：

"GameplayAbilities", "GameplayTags","GameplayTasks"
    
```

```c++
//多人游戏 的服务器组成
```

![Screenshot_20240411_080941](.\photos\Screenshot_20240411_080941.jpg)

```c++
//不同情况下的适用情况
```

![](.\photos\Screenshot_20240411_082841.jpg)

```c++
//初始化Actor信息

//Enemy
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponement->InitAbilityActorInfo(this, this);

}



//Player Controller Character
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//在服务器端进行初始化
	InitialAbilityActor();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//在客户器端进行初始化
	InitialAbilityActor();
}
/*
* 在实际操作中反复没有反应原因是AuraPlayerState在UE中没有写蓝图类，放入GameMode中
**/
void AAuraCharacter::InitialAbilityActor()
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		AuraState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraState, this);
		AbilitySystemComponement = AuraState->GetAbilitySystemComponent();
		AttributeSet = AuraState->GetAttributSet();
	}
}

```

![Screenshot_20240411_084010](.\photos\Screenshot_20240411_084010.jpg)

## 7.AttributeSet的实现

```c++
//Attribute实现前可以看下内置的头文件
#include "AttributeSet.h"
//初始化属性成员和复制函数OnRep_Attribute()中的提前设置都在里面


//包含属性的GetHealthAttribute();GetHealth();SetHealth(float NewVal);InitHealth(float NewVal);

/**
 * This defines a set of helper functions for accessing and initializing attributes, to avoid having to manually write these functions.
 * It would creates the following functions, for attribute Health
 *
 *	static FGameplayAttribute UMyHealthSet::GetHealthAttribute();
 *	FORCEINLINE float UMyHealthSet::GetHealth() const;
 *	FORCEINLINE void UMyHealthSet::SetHealth(float NewVal);
 *	FORCEINLINE void UMyHealthSet::InitHealth(float NewVal);
 *
 * To use this in your game you can define something like this, and then add game-specific functions as necessary:
 * 
 *	#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 * 
 *	ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)
 */




/**
 *	This is a helper macro that can be used in RepNotify functions to handle attributes that will be predictively modified by clients.
 *	
 *	void UMyHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
 *	{
 *		GAMEPLAYATTRIBUTE_REPNOTIFY(UMyHealthSet, Health, OldValue);
 *	}
 */


#include "AuraAttributeSet.h"
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attribute")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth, Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attribute")
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attribute")
	FGameplayAttributeData MaxMana;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana);
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);


#include "AuraAttributeSet.cpp"

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}


//GAMEPLAYATTRIBUTE_REPNOTIFY这个宏用于RepNotify函数中，在客户端中修改属性
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
```

## 8.血药的制作

```c++
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	//绑定碰撞
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlapEnd);
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (AbilitySystemInterface)
	{
		//临时的：不规范强行将const类型转换为非const类型
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AbilitySystemInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* TeamAttribute  = const_cast<UAuraAttributeSet*>(AuraAttributeSet);

		TeamAttribute->SetHealth(TeamAttribute->GetHealth() + 25.f);
	}
	Destroy();
	//Destroyed();
}

void AAuraEffectActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

```

## 9.Widget和WidgetController

```c++
#include "WidgetController.h"
//存储4个带有信息的数据

//方便获取各种信息来传递给Widget
UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
TObjectPtr<APlayerController> PlayerController;

UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
TObjectPtr<APlayerState> PlayerState;

UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
TObjectPtr<UAbilitySubsystemComponent>AbilitySubsystemComponent;

UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
TObjectPtr<UAttritubeSet>AttributeSet;

#include "Widget.h"

//从Controller获取信息
UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
TObjectPtr<UObject> WidgetController;
```

![image-20240411232546449](.\photos\image-20240411232546449.png)

## 10.血量进度条Widget的制作(蓝图Blueprint)

```c++
//变量信息
```

<img src=".\photos\image-20240412180016673.png" alt="image-20240412180016673" style="zoom:25%;" />



```c++
//函数的内部都是设置图片大小类型什么的
```

<img src=".\photos\image-20240412180038737.png" alt="image-20240412180038737" style="zoom:25%;" />

<img src=".\photos\image-20240412180053077.png" alt="image-20240412180053077" style="zoom:25%;" />

<img src=".\photos\image-20240412180104529.png" alt="image-20240412180104529" style="zoom:25%;" />

<img src=".\photos\image-20240412180118038.png" alt="image-20240412180118038" style="zoom:25%;" />



## 11.设置HUD和一些属性初始化的操作

```c++
//OverlapWidgetController是基于AuraWidgetController
//为了使AuraWidgetController成为基类


//在WidgetController里面写4个参数方便以后进行参数的获取同时为了让他作为基类，
//写了一个结构体FWidgetControllerParams方便4个参数的输出存储，
//并写SetWidgetControllerParams()使类中4个参数进行初始化赋值
//创建子类OverlapWidgetController
//创建一个AuraHUD在AuraHUD中将会AddToViewport()
//需要参数UserWidget和UserWidgetClass
//并且想要和OverlapWidgetController建立联系参数为OverlapWidgetController和OverlapWidgetControllerClass
//先写个单例模式将OverlapWidgetController进行初始化
UOverlapWidgetController* AAuraHUD::GetOverlapWidgetController(const FWidgetControllerParams& WCParams);
//后面写将Controller和Widget联系起来的函数
void AAuraHUD::InitOverlap(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)；
//先检查两个Class是否有值checkf()
//对UserWidget初始化
//将UserWidget转化为AuraUserWidget
//创建OverlapWidgetController并进行初始化
//并调用AuraUserWidget里面的SetWidgetController进行联系


UOverlapWidgetController* AAuraHUD::GetOverlapWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlapWidgetController == nullptr)
	{
		OverlapWidgetController = NewObject<UOverlapWidgetController>(this,OverlapWidgetControllerClass);
		OverlapWidgetController->SetWidgetControllerParams(WCParams);

		return OverlapWidgetController;
	}
	return OverlapWidgetController;
}

void AAuraHUD::InitOverlap(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlapWidgetClass, TEXT("Set OverlapWidgetClass In BP_AuraHUD"));
	checkf(OverlapWidgetControllerClass, TEXT("Set OverlapWidgetControllerClass In BP_AuraHUD"));


	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlapWidgetClass);
	OverlapWidget = Cast<UAuraUserWidget>(Widget);
	//将Widget和WigetController结合再一块
	FWidgetControllerParams ControllerParmas(PC, PS, ASC, AS);
	UOverlapWidgetController* WigetController = GetOverlapWidgetController(ControllerParmas);
	OverlapWidget->SetWidgetController(WigetController);

	Widget->AddToViewport();
}




#include "AuraWidgetController.h"
USTRUCT(BlueprintType)
struct FWidgetControllerParams 
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		:PlayerController(PC), PlayerState(PS), AbilitySubsystemComponent(ASC), AttributeSet(AS){};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent>AbilitySubsystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet>AttributeSet = nullptr;

};
```

## 12.绑定生命回调函数并实现在生命进度条上

```c++
//委托的宣称
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSingnature, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSingnature, float, MaxHealth);



//两个参数的委托变量
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnHealthChangedSingnature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnMaxHealthChangedSingnature OnMaxHealthChanged;


//需要绑定的函数来时刻调整绑定参数的值
	void HealthChange(const FOnAttributeChangeData& Date)const ;

	void MaxHealthChange(const FOnAttributeChangeData& Date)const;




//对其进行委托绑定的参数初始化赋值
void UOverlapWidgetController::InitBroadcast()
{
	UAuraAttributeSet*AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}

//将函数和参数进行绑定来时刻调整参数值
void UOverlapWidgetController::BroadcastCallbackToDependences()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlapWidgetController::HealthChange);
	AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlapWidgetController::MaxHealthChange);

}

void UOverlapWidgetController::HealthChange(const FOnAttributeChangeData& Date)const
{
	OnHealthChanged.Broadcast(Date.NewValue);
	
}

void UOverlapWidgetController::MaxHealthChange(const FOnAttributeChangeData& Date)const
{
	OnMaxHealthChanged.Broadcast(Date.NewValue);
}

```

## 13.Gameplay Effects

```c++
//写ApplyEffectToTarget函数

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffect)
{
	UAbilitySystemComponent* ASCTarget = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (ASCTarget == nullptr)return;
	FGameplayEffectContextHandle EffectContextHandle = ASCTarget->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASCTarget->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContextHandle);
	ASCTarget->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}


```

### 13.1.GameplayEffect的不同模式(Instant和Duration)

```c+
//Instant 即刻生效
//Has Duration 延迟生效
//Infinite 永久生效
//Period 周期性生效(可与上面的进行组合)

//Period 每多少秒生效一次
```

### 13.2.GameplayEffect的Stacking Type

```
//Stack 加的buff层数层数越多效果越好

//Stacking Type: Aggregate by Soure 与 Aggregate by Target
//相同点:主要限制捡起的数目和Stack Limit Count限制数目
//不同点:在与Stack的对象

//Soure 是来限制可以捡起的某一种EffectActor数量
//Target 是来限制Character可以捡起的数目


//Stack Expiration Policy 对于捡起的对象过期后的处理方案
	//Clear Enter Stack 一次性全部清空
	//Remove Single Stack And Refresh Duration 一次掉一层剩下的继续重复生效
	//Refresh Durtion 不会清除层数
```

### 13.3.InfiniteEffect的取消

```c++
void AAuraEffectActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UAbilitySystemComponent* ASCTarget = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASCTarget == nullptr)return;

	TArray<FActiveGameplayEffectHandle> RemoveEffectHandleMap;
	for (auto& EffectHandle : ActiveEffectHandleMap)
	{
		if (ASCTarget == EffectHandle.Value)
		{
            //在此处消除
			EffectHandle.Value->RemoveActiveGameplayEffect(EffectHandle.Key,1);
			RemoveEffectHandleMap.Add(EffectHandle.Key);
		}	
	}

	for (auto& EffectHandle : RemoveEffectHandleMap)
	{
		ActiveEffectHandleMap.FindAndRemoveChecked(EffectHandle);
	}

}
```



### 13.4.Per和Post GameEffectChange应用

```c++
//在数据生效前和生效后调用的函数

virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;
virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)override;

//属性生效前调用
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//NewValue is a mutable reference so you are able to clamp the newly applied value as well

	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}

}
//属性生效后调用
void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
```

### 13.5.PostGameplayEffectExecute

```c++
//在修改属性之前提前进行属性修改,修改实际值
virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectParams EffectParams;

	SetGameplayEffectPramars(Data, EffectParams);
	//预处理函数中值修改仍要在应用时进行修改
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
	}

}

//可以相互调用
AbilitySystemComponent<-GameplayEffectContextHandle<-GameplayEffectSpecHandle;
  

//P45
void UAuraAttributeSet::SetGameplayEffectPramars(const FGameplayEffectModCallbackData& Data, FGameplayEffectParams& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetEffectContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (Props.SourceASC && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
		}
	}


	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);

		if (Props.TargetController == nullptr && Props.TargetAvatarActor)
		{
			if (APawn* Pawn = Cast<APawn>(Props.TargetAvatarActor))
			{
				Props.TargetController = Pawn->GetController();
			}
		}
		if (Props.TargetController)
		{
			Props.TargetCharacter = Props.TargetController->GetCharacter();
		}
	}
}

```

## 14.Gameplay Tags

### 14.1.Tags的创建

```c++
//2种方式 直接创建和间接创建

//直接创建
//在  设置-项目设置-GameplayTags-管理GameplayTags中添加

//间接创建
//先创建 GameplayTagsTable 完善TagsTable
//在 设置-项目设置-GameplayTags-GameplayTag列表中添加

```

### 14.2.Tag的简单GameEffect的应用

```c++
//可以在GameplayEffect中添加Tags对触发角色添加Tags或对自身添加Tags

//在GameplayEffect 中Components添加新属性

//Tags This Effect Has(Assert Tags)   	是对EffectActor自身添加的属性
//Grant Tags To Target Actor 			添加到目标Actor上

	/** Called on server whenever a GE is applied to self. This includes instant and duration based GEs. */
	FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf;
//用委托来显示GameEffect触发时的 Effect信息

//打印信息到屏幕
GEngine->AddOnScreenDebugMessage()；
    

```

### 14.3.DataTable和Tag的结合

```c++
//C++中声明一个结构体继承于FTableRowBase ：FUIWidgetData
//UE中创建DataTable时可以选择 FUIWidgetDate
//拾取物品时的信息播报需要的信息在struct中写出在UE中填写

//绑定Lambda表达式
Cast<UAuraAbilitySystemComponent>(AbilitySubsystemComponent)->AssertTag.AddLambda(
    //这里的this是在Lambda函数中出现类中的成员时需要的
	[this](const FGameplayTagContainer& Container)
	{
		for (const FGameplayTag& Tags : Container)
		{
			//判断Tags是否满足 MessageTag
			//"Message.HealthPotion".MatchesTag("Message") will return True, 
			//"Message".MatchesTag("Message.HealthPotion") will return False
			//筛选出Message的Tag
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tags.MatchesTag(MessageTag))
			{
				checkf(MessageDataTable, TEXT("Please Set MessageDataTable in BP_OverlapWidgetController First"));
                //寻找对应的Row
				FUIWidgetData* WidgetData = MessageDataTable->FindRow<FUIWidgetData>(Tags.GetTagName(), TEXT(""));
                //在UE蓝图中有个事件来让其触发,来生成相应的Widget和图片
                OnMessageWidgetSingnature.Broadcast(*WidgetData);
			}
		}
	}
);


```

### 14.4.拾取消息的出现和消失

```c++
//出现完全在蓝图中实现

//AuraAbilitySystemComponent中的
//GameplayEffectAppliedDelegateToSelf函数中
//执行了AssertTag.Broadcast(Container);
//调用了在OverlapWidgetController中的绑定的Lambda表达式
//后在蓝图WBP_AuraOverlap中Assign了事件
//进行执行




//通过datatable中填写信息 后生成相应的widget再AddToView()实现


//用Delay()和RemoveFromParent()蓝图实现消失


```

### 14.5.拾取消息的UI的动画蓝图

```c++
//在相应的Widget中创建动画，UE会将这些动画变成一个动画名的变量(能通过相应的Widget去调用这个变量)
//通过PlayAnimation去播放这个动画即可
```

### 14.6.通过Effect初始化属性值

```c++
void AAuraCharacterBase::InitialAttribute() const
{
	InitialGameEffect(PrimaryEffectClass);
	InitialGameEffect(SecondaryEffectClass);
}

void AAuraCharacterBase::InitialGameEffect(TSubclassOf<UGameplayEffect> EffectClass) const
{
	checkf(PrimaryEffectClass, TEXT("Please check  Character Blueprint is add EffectClass Blueprint"));

	if (IsValid(GetAbilitySystemComponent()))
	{
        //应用GameEffect对角色
		const FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, 1.f, EffectContextHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
	}
}

```

### 14.7.Effect的Modifiers基于Attribute Base

```c++
//Attribute Base 中 Atrribute Based Magnitude 基本运算
//
//Base Value
//Coefficient 
//Pre Multiply Additive Value
//Post Multiply Additive Value
//计算公式
// Value (+ - * /) = (Base Value + Pre Multiply Additive Value) * Coefficient + Post Multiply Additive Value;

```

### 14.8.MMC(Modifier Magnitude Calculation)通过C++来完成属性基于不同元素的计算

```c++
//PlayerState里面的Level和Attribute中的Vigour共同计算MaxHealth在MMC_MaxHealth中
//Level： Character Level , Enemy Level

//Enemy Level 是固定的不会发生变化
//故Enemy的Level放在EnemyCharacter中 继承的GetPlayerLevel 直接return level
int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

//Character Level 是会发生变化的 且
//我们放在PlayerState中为复制变量故Character Level要通过PlayerState去GetPlayerLevel
int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraState, TEXT("Please Set BP_AuraPlayerState in BP_AuraGameMode First"));
	return AuraState->GetPlayerLevel();
}


//MMC

//捕获游戏属性
FGameplayEffectAttributeCaptureDefinition VigourDef;

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//获取Vigour属性
	VigourDef.AttributeToCapture = UAuraAttributeSet::GetVigourAttribute();
	VigourDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigourDef.bSnapshot = false;
	//加入捕获表中
	RelevantAttributesToCapture.Add(VigourDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{	
    //FAggregatorEvaluateParameters 一般没用 就是加tag信息的
	const FGameplayTagContainer* SoureTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TagertTag= Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SoureTag;
	EvaluateParameters.TargetTags = TagertTag;
	
    //获取Vigour值
	float Vigour = 0.f;
	GetCapturedAttributeMagnitude(VigourDef, Spec, EvaluateParameters, Vigour);
	Vigour = FMath::Max(Vigour, 0.f);

	//获取Level
	checkf(Spec.GetContext().GetSourceObject(), TEXT("Please AddSourceObject in ApplyEffect Class "));
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 Level = CombatInterface->GetPlayerLevel();


	return 80.f + 10 * Level + 2.5 * Vigour;
}
```

## 15.Attribute Menu菜单制作

### 15.1.UI的设计

```c++

```

![屏幕截图 2024-05-31 233117](.\photos\屏幕截图 2024-05-31 233117.png)

### 15.2.数据的更新

```c++
//将变化的Attribute数据更新到WBP_AttributeMenu中

//1.设置AtrributeTag在C++中
//需要一个存储Tag信息的位置FGameplayTags
//运用以下函数进行数据存储
UGameplayTagsManager::Get().AddNativeGameplayTag(FName TagName, const FString& TagDevComment);

//还有一个即真正加入Tag中的到AssetManager 		UAssetManager
//在下面函数调用Tag的初始化即   Tag函数存储  	⬆
virtual void StartInitialLoading() override;

```



### 15.3.DataAsset

```c++
//2.Data Asset 获取Tag信息


//存储相应信息的结构体
USTRUCT(BlueprintType)
struct FDataAssetInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag GameplayTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TagName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TagComment = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

//在蓝图中填写相应的信息
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
TArray<FDataAssetInfo>  AttributeInfomation;




//通过信息查询值
FDataAssetInfo UAuraDataAsset::FindAttributeInfoFromTag(const FGameplayTag& AttributeTag, bool bNotLogFind) const
{

	for (const FDataAssetInfo& DataInfo : AttributeInfomation)
	{
		if (DataInfo.GameplayTag.MatchesTagExact(AttributeTag))
		{
			return DataInfo;
		}
	}

	if (bNotLogFind)
	{
		UE_LOG(LogTemp, Error, TEXT("Not FInd Attribute [%s] In AuraDataAsset [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FDataAssetInfo();
}

```



### 15.4.BlueprintFunctionLibrary

```c++
//Library类中主要写Get相应的WidgetController
//在c++中创建能在UE蓝图中使用的函数

//BlueprintPure 纯函数，没有白色引脚，必须要有输入输出值

UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|AbilitySystemLibrary")
static UOverlapWidgetController* GetOverlapWidgetController(UObject* WorldContext);


UOverlapWidgetController* UAuraAbilitySystemLibrary::GetOverlapWidgetController(UObject* WorldContext)
{
	
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0)))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			if (AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
			{
				UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
				UAttributeSet* AS = PS->GetAttributSet();
				FWidgetControllerParams Params(PC, PS, ASC, AS);
				// 获取4个参数来获取 OverlapWidgetController
				return AuraHUD->GetOverlapWidgetController(Params);
			}
		}
	}
	return nullptr;
}

```

### 15.5.将AttributeMenuWidget装载完毕

```c++
//AttributeMenuWidget 载入AuraHUD中 与OverlapWidget一样
//同时加入AbilitySystemLibrary中

//主要目的将Widget的Controller进行指定
```

### 15.6.将数据导入WBP_AttributeMenu中

```c++
//首先在WBP_AttributeMenu中调用InitBroadcast

////然后写委托绑定事件即可



void UAttributeMenuWidgetController::InitBroadcast()
{
    //通过AttributeSet获取数据
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	checkf(AssetInfo, TEXT("检查 AssetInfo 是否加入到 蓝图AttributeMenuWidgetController中"));
	//Info是要播报的数据
	FDataAssetInfo Info = AssetInfo->FindAttributeInfoFromTag(FAuraGameplayTags::Get().Attribute_Primary_Strength);

	Info.AttributeValue = AuraAttributeSet->GetStrength();

	AttributeInfoDelegate.Broadcast(Info);
}

//为了使WBP_AttributeMenu中每个单独的Row对应相应的名称和数据对其添加GameplayTag属性进行一对一绑定

//常规绑定方法
	//用一个Delegate去播报信息
	FDataAssetInfo Info = AssetInfo->FindAttributeInfoFromTag(FAuraGameplayTags::Get().Attribute_Primary_Strength);
	Info.AttributeValue = AuraAttributeSet->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);


//返回值类型为FGameplayAttribute的一个委托,可以通过委托获取其绑定的FGameplayAttribute
DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);
//运用Map将FGameplayTag和FAttributeSignature放在一起
TMap<FGameplayTag, FAttributeSignature> TagToAttribute;
//然后将FGameplayAttribute进行绑定到FAttributeSignature并添加到map中
FAttributeSignature StrengthDelegate;
StrengthDelegate.BindStatic(GetStrengthAttribute);

TagToAttribute.Add(AuraGameplayTags.Attribute_Primary_Strength, StrengthDelegate);

//最后在MenuController中遍历map并进行播报

	for (auto& p : AuraAttributeSet->TagToAttribute)
	{
		FDataAssetInfo Info = AssetInfo->FindAttributeInfoFromTag(p.Key);
		Info.AttributeValue = p.Value.Execute().GetNumericValue(AuraAttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}


//再优化：将相应数据Add进入map的代码过于繁琐
//通过函数指针获得对应的Attribute函数可以快速得到Value的值
TMap<FGameplayTag, FGameplayAttribute(*)()> TagToAttribute;
TagToAttribute.Add(AuraGameplayTags.Attribute_Primary_Strength, GetStrengthAttribute);


	for (auto& p : AuraAttributeSet->TagToAttribute)
	{
		FDataAssetInfo Info = AssetInfo->FindAttributeInfoFromTag(p.Key);
		Info.AttributeValue = p.Value().GetNumericValue(AuraAttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}

//别名模板
template<class T>
using TAttributePtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;
TMap<FGameplayTag,TAttributePtr<FGameplayAttribute()>> TagToAttribute;
//与上面的没什么区别，多了一个FDefaultDelegateUserPolicy暂时不知道有什么用
```

### 15.7.BroadcastCallbackToDependences的完善

```c++
//为每个Attribute的value绑定委托
for (const auto& Pair : AuraAttributeSet->TagToAttribute)
{
    //Lambda表达式绑定Delegate和Pair.Value()
	AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair, AuraAttributeSet](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
	);
}

//循环里的出现多次提取出来 记得在头文件中先声明FGameplayTag和FGameplayAttribute变量
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FDataAssetInfo Info = AssetInfo->FindAttributeInfoFromTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

```

## 16.Gameplay Ability

### 16.1.GameplayAbility的初认识

```c++
//Gameplay Ability
//一般被用来定义一个能力和技能
//必须在服务端中被授权,且规范的复制到自身的客户端中
//必须要激活才能使用
//需要有花费和冷却时间
//Gameplay Ability可以在执行期间做其他任何事(异步)
//可以同时多个Gameplay Ability一起运行
//Gameplay Ability运行的Ability Taks
```

### 16.2.GameplayAbility的实际使用Active和End状态

```c++

```

### 16.3.Ability中的Gameplay Tags

```c++
//Cancel Ability:
//使用这个Tag的Abilities将会在这个Ability执行时取消
//Block Ability:
//当使用这个Tag的Ability处于active状态时其他Abilities被阻止
//Activation Owned Tags
```

![image-AbilityTag](.\photos\AbilityTag.png)

![image-20240627234829916](.\photos\InstancingPolicy.png)

![image-20240627235105517](.\photos\NetExecutionPolicy.png)

### 16.4.Ability的输入使用了DataAsset

```c++
//Data Asset和Enhanced Input结合

//Data Asset中存储UInputAction和对应的GameplayTag信息

//新建EnhancedInputComponent类写绑定函数
template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HoldFuncType>
void UAuraInputComponent::BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HoldFuncType HoldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction& Action: InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.GameplayTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.GameplayTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.GameplayTag);
			}

			if (HoldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HoldFunc, Action.GameplayTag);
			}
		}
	}
}



//在Controller中的调用
	AuraInputComponent->BindAbilityAction(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHold);

//在Pressed，Released，Hold函数中根据Tag来写指定的功能

```

### 16.5.自动寻路

```c++
//UE中  放置Actor中添加一个体积 - NavMeshBoundsVolume
//可以按P查看范围

#include "NavigationPath.h"

UNavigationPath
 
//生成路径    
UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CacheHitVector)
    
//客户端 执行导航路线FindPathToLocationSynchronously需要在项目设置NavigationSystem中 把运行客户端导航打开
    
    
    
    
APawn* ControlledPawn = GetPawn();
if (ControlledPawn&& FollowTime <= ShortPressTime)
{
	if (UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CacheHitVector))
	{

		Spline->ClearSplinePoints();
		for (const FVector& PointLoc : Path->PathPoints)
		{
			Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
			DrawDebugSphere(GetWorld(), PointLoc, 5.f, 32, FColor::Green, true, 5.f);
		}
		//更新鼠标点击点防止鼠标点到不可到达地人物不停止移动
		if (!Path->PathPoints.IsEmpty())
		{
			CacheHitVector = Path->PathPoints[Path->PathPoints.Num() - 1];
		}
		bAutoRuning = true;
	}
}
```

### 16.6.关于这个项目的Ability详解

#### 需求:

有一个存储InputAction和对应Tag的Data,有一些自己定义的AuraAbility里面有InputTag,有一个存储这些Abilities的数组 并在角色类中

#### 1.需要让Tag进行绑定Action

所有我们写了一个函数进行绑定按键和函数

BindAction()进行绑定按键和函数

```c++
BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.GameplayTag);
//@Param:
//Action.InputAction 对应按键
//ETriggerEvent::Started 触发效果
//Object 对应的Class 哪个类进行绑定
//PressedFunc 需要执行的函数
//Action.GameplayTag 函数所需要的参数信息
```

现在每个按键都有自己的Tag只需要按下相应的按键就会触发相应的函数

#### 2.我们需要在需要执行的函数中激活Ability即可



首先我们需要将Ability先赋予角色进行激活让角色拥有这个Ability

```c++
//进行标签的绑定,自定义的Tag加入到Ability的动态标签中
AbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->InputGameplayTag);
//将Ability赋予给相应的角色
GiveAbility(AbilitySpec);
```

当我们按下相应的按键会触发绑定的函数我们在函数中会找角色被给予的Ability中是否有相同的InputTag，

需要找到InputTag相同的Ability

```c++
AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)
```

如果有就激活它TryActivateAbility();

## 17.火球(FireBall)

### 17.1.火球技能

```c++
//创建火球
//写好碰撞SphereComponent
//写好移动ProjectileMovementComponent

//碰撞和碰撞的函数进行绑定在BeginPlay绑定
/** Delegate for notification of start of overlap with a specific component */
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_SixParams(FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult);

SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::ProjectileBeginOverlap);

//碰撞的绑定函数需要为  UFUNCTION();
```

### 17.2.火球在动画播放中生成(Notify)

```c++
//火球的生成

//火球生成的时机(播放Montage中间某个时刻执行)(使用了Notifies)

//创建一个蓝图AnimNotify重写函数Receive Notify

//Send Gameplay Event To Actor函数 和 GA_FireBall 中的Wait Gameplay Event来传递信息通知GA去生成FireBall
```

![image-20241201004810854](.\photos\Notify.png)

### 17.3.火球的生成的方向

```c++
//人物朝向获取鼠标点击点的坐标
//写一个Task类中去获取坐标
UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "CreateTargetData",HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
static UTargetDataUnderMouse* CreateUnderMouseTargetData(UGameplayAbility* OwningAbility);


//Task类中获取坐标的函数 实现多引脚
//写一个委托就实现了多引脚
//通过委托传递数据到蓝图中
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseDataTargetSingnature, const FGameplayAbilityTargetDataHandle&,DataHandle);

//坐标的信息需要从客户端传递信息到服务端
//传递Data到服务端

FGameplayAbilityTargetDataHandle DataHandle;
FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
Data->HitResult = HitResult;
DataHandle.Add(Data);

AbilitySystemComponent->ServerSetReplicatedTargetData(
	GetAbilitySpecHandle(), 
	GetActivationPredictionKey(), 
	DataHandle, 
	FGameplayTag(), 
	AbilitySystemComponent->ScopedPredictionKey);


//服务端去使用数据
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
    //使用来自客户端的缓存 TargetData（仅 TargetData）
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//将Location通过委托传递给蓝图
		ValidData.Broadcast(DataHandle);
	}
}



//绑定的函数在何时调用？//再等待之后执行绑定的函数
AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

//数据未成功传递，需要等待
if (!AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey))
{
	SetWaitingOnRemotePlayerData();
}


#include "AbilitySystemGlobals.h"
//当使用Target Data必须要添加的
UAbilitySystemGlobals::Get().InitGlobalData();
```

### 17.4.人物朝向(motion warping)

```c++
//一个插件motion warping
//添加在人物蓝图中
//可以改变人物根运动

//动画中添加通知，并写好名字，设置只改变方向
//在人物蓝图中写好事件
//用motion warping中的

//add or updata warp target from location()
//根据target location改变方向
```

### 17.5.火球碰撞

```c++
//考虑碰撞发生时间
//服务器碰撞和客户端碰撞先后 造成不同的影响

//声音特效和持续时间

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

UNiagaraFunctionLibrary::SpawnSystemAtLocation();
UGameplayStatics::PlaySoundAtLocation();

//避免和药水等物品发生碰撞
//项目设置中
```

### 17.6.火球伤害

```c++
//通过GameplayEffect实现的

//在火球执行OnBeginOverlap中应用伤害到目标上
//应用GameplayEffect调用AbilitySystemComponent中ApplyGameplayEffectSpecToSelf
//需要一个变量FGameplaySpecHandle
//因为生成FGameplaySpecHandle时候
SoureASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SoureASC->MakeEffectContext());
//需要一个Level且Ability中含有Level故在Ability文件中去生成

UAbilitySystemComponent* SoureASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
if (SoureASC)
{
	SoureASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

```

## 18.敌人血条

### 18.1.血条Widget的制作

```c++
//先制作一个Widget含有Bar


//添加到Character中
```

### 18.2.血条的消失和显示

```c++
//设置Visibility和Delay即可
```



## 19.对不同职业的角色进行初始化属性

### 19.1.Data的创建

```c++
//Data创建是对不同角色的初始值进行初始化

//哪些值需要被初始化：PrimaryAttribute , VitalAttribute ,SecondaryAttribute


```

### 19.2.CurveTable的创建

```c++
//导出csv文件进行编辑再进行导入

//在UE中导入csv文件选择CurveTable进行创建
```

### 19.3.DataAsset的使用

```c++
//由于DataAsset仅需在一个地方出现，则选择放在GameMode中

//Init函数则选择放在自己写的库中方便使用
void UAuraAbilitySystemLibrary::InitDefaultAttribute(const UObject* WorldContext, ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));

	if (AuraGameMode == nullptr|| ASC == nullptr)return;

	UCharacterClassInfo* ClassInfo = AuraGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetCharacterDefaultInfo(CharacterClass);

	const AActor* AvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle PrimarySpecHandle = ASC->MakeEffectContext();
	PrimarySpecHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, level, PrimarySpecHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondarySpecHandle = ASC->MakeEffectContext();
	SecondarySpecHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, level, SecondarySpecHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalSpecHandle = ASC->MakeEffectContext();
	VitalSpecHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, level, VitalSpecHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());

}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//在使用Effect时
ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());

//需要MakeOutgoingSpec和MakeEffectContext
//由于使用了MMC，在MakeEffectContext需要AddSoureObject
	FGameplayEffectContextHandle VitalSpecHandle = ASC->MakeEffectContext();
	VitalSpecHandle.AddSourceObject(AvatarActor);
//------------------------------------------------------------------------------------------------------------------------------------------------------
	
```

## 20.伤害和受伤反馈

### 20.1.新属性 Damage 伤害和玩家属性结合

```c++
//关键函数
//添加新属性FGameplayAttributeData

//为了在GameplayEffect中使用


//需要一个EffectSpecHandle和一个Tag

//让EffectSpec绑定 伤害和Tag
UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude();



```

### 20.2.收到伤害时做出反应

```c++
//通过播放动画来实现

//先将Ability赋予给相应的角色
GiveAbility();

//Actor在收到伤害时被赋予了相应的Tag，通过这个Tag去激活特定的Ability在Ability中先对Actor激活Effect再进行Montage的播放再去除这个Effect并结束Ability

TryActivateAbilitiesByTag();

FGameplayTagContainer TagContainer;
TagContainer.AddTag(FAuraGameplayTags::Get().Combat_HitReact);
EffectParams.TargetASC->TryActivateAbilitiesByTag(TagContainer);


```

### 20.3.Enemy的死亡和材质溶解

```c++
//死亡Death
//和组件分离时并保留在世界上
WeaponMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));

//重力设置和启用物理不要忘记
GetMesh()->SetEnableGravity(true);
GetMesh()->SetSimulatePhysics(true);

//死亡要注意在服务端和客户端，服务端进行死亡，客户端设置Collision


//溶解Dissolve

//Material的替换和TimeLine将Dissolve数值进行提升就行,TimeLine是蓝图函数需要
UFUNCTION(BlueprintImplementableEvent)
void StartDissolveMeshAndWeaponTimeLine(UMaterialInstanceDynamic* DissolveMeshMaterial, UMaterialInstanceDynamic* DissolveWeaponMeshMaterial);

void AAuraCharacterBase::Dissolve()
{
	if(IsValid(DissolveMeshMaterialInstance)&&IsValid(DissolveWeaponMeshMaterialInstance))
	{
		UMaterialInstanceDynamic* MeshMaterial = UMaterialInstanceDynamic::Create(DissolveMeshMaterialInstance, this);
		GetMesh()->SetMaterial(0, MeshMaterial);
		UMaterialInstanceDynamic* WeaponMeshMaterial = UMaterialInstanceDynamic::Create(DissolveWeaponMeshMaterialInstance, this);
		WeaponMesh->SetMaterial(0,WeaponMeshMaterial);
		if (MeshMaterial && WeaponMeshMaterial)
		{
			StartDissolveMeshAndWeaponTimeLine(MeshMaterial, WeaponMeshMaterial);
		}
	}
}
```

### 20.4.伤害显示

```c++
//进行Widget的添加


//TSubclassOf<UFloatingTextWidgetComponent> 转换为  UFloatingTextWidgetComponent* 类型
UFloatingTextWidgetComponent* DamageText = NewObject<UFloatingTextWidgetComponent>(this, DamageWidgetClass);

//关键函数在蓝图中进行
UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
void SetFloatingText(float DamageText);


void AEnemyCharacter::ClientShowFloatingDamageText_Implementation(float DamageFloat)
{
	if (DamageWidgetClass)
	{
		UFloatingTextWidgetComponent* DamageText = NewObject<UFloatingTextWidgetComponent>(this, DamageWidgetClass);
		if (DamageText)
		{
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			
          
			DamageText->SetFloatingText(DamageFloat);
		}
	}
}


```

### 20.5.关于伤害和其他属性结合的修改捕获属性(ExecuteCalculation)

```c++

//捕获属性的宏,写在自己建立的结构体中
struct Damage
{
    //声明要捕获的属性
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    
    Damage()
    {
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
    } 
};
//static 函数防止声明多个捕获的属性
static const Damage& DamageStatics()
{
	static Damage DStatic;
	return DStatic;
}

//需要添加进行捕捉属性,在构造函数中
RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);

//将捕获的属性的值赋予MagicalDefense
ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalDefenseDef, EvaluateParameters, MagicalDefense);


//输出最终的值Damage
const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage);
OutExecutionOutput.AddOutputModifier(EvaluatedData);



struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalDefense);

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, MagicalDefense, Target, false);
	}
};

```

### 20.6.曲线在C++中数据的调用

```c++
UCurveTable
    ->
FindCurve(FName("曲线名",FString));
```

## 21.自定义FGameplayEffectContext

### 21.1.GameplayContext的内容

**自己添加的.cpp和.h需要自行添加到Source文件夹中并删除Save、Binary、Intermediate文件后重新生成解决方案**

```c++
//需要继承的函数
//在struct FGameplayEffectContext中必须继承 


	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}


	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

```

### 21.2.完善NetSerialize函数

```c++
//在FGameplayEffectContext结构体中可以参考其函数
bool FGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint8 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
	}

	Ar.SerializeBits(&RepBits, 7);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}


//函数的目的主要为了存储 bool数据 和 将存储的数据进行读取,
//每个位置的01代表一个bool

//添加自己需要的bool变量并添加到函数中
if (bCritical)
{
	RepBits |= 1 << 7;
}

if (RepBits & (1 << 7))
{
	Ar<<bCritical;
}
```

### 21.3.不知道什么功能的Duplicate和TStructOpsTypeTraits

```c++
/** Creates a copy of this context, used to duplicate for later modifications */
virtual FGameplayEffectContext* Duplicate() const
{
	FGameplayEffectContext* NewContext = new FGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult())
	{
		// Does a deep copy of the hit result
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}



template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
```

### 21.4.应用自定义AuraGameplayEffectContext

```c++
//创建UAbilitySystemGlobals的子类UAuraAbilitySystemGlobals

//继承
/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;


//在DefaultGame.ini中增加
[/Script/GameplayAbilities.AbilitySystemGlobals]
+AbilitySystemGlobalsClassName = "/Script/Aura.AuraAbilitySystemGlobals"

//在蓝图中设置bool

    
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameEffectContext")
	static bool IsCriticalHIt(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameEffectContext")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bCriticalHit);

    //在蓝图中非const的&变量是输出端的如果想要成为输入端需要UPARAM(ref)

```

### 21.5.设置显示数值的颜色

```c++
//在蓝图中通过bool属性进行改变颜色即可

```

## 22.DamageType

### 22.1.DamageType

```c++
//通过创建Tarray 实现
#include "AuraGameplayAbility.h"
Tarray<FScalableFloat> Damages;
```

### 22.2.Map<Damage,Resistance>

```c++
//通过绑定相应的抵抗值和伤害可以在计算伤害时更好的应用

```

## 23.Enemy AI

### 23.1.AI的构成

敌人的AI由
`Enemy`中含有的`AIController`和`BehaviorTree`

```c++
virtual void PossessedBy(AController* NewController) override;


void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	AuraAIController = Cast<AAuraAIController>(NewController);
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AuraAIController->RunBehaviorTree(BehaviorTree);
	}
}
```

`AIController`中含有`BehaviorTreeComponent`和`BlackboardComponent`构成



其中C++中`AIController`含有`BlackboardComponent`不需要自己声明

```c++
/** blackboard */
UPROPERTY(BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
TObjectPtr<UBlackboardComponent> Blackboard;
```

`BehaviorTree`中需要添加`BlackboardComponent`

### 23.2.Behavior Tree Servive

可以调控BehaviorTree的触发时间

```c++
virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
```

C++中定义的属性

```c++
//连接Blackboard中属性和Behavior Tree Servive中自己定义的属性
UPROPERTY()
FBlackboardKeySelector
```

每个Actor都可以在蓝图中设置自己的Tag可以通过Tag获取相同Tag的Actor

```c++
//获取敌对目标的Target
const FName TargetTag = AIPawn->ActorHasTag("Player") ? FName("Enemey") : FName("Player");

TArray<AActor*> TargetActors;
//获取Actors
UGameplayStatics::GetAllActorsWithTag(AIPawn, TargetTag, TargetActors);
```

`GetDistanceTo`

```c++
//找最近的玩家
for (AActor*& TargetActor : TargetActors)
{
	const float DistanceToTarget = AIPawn->GetDistanceTo(TargetActor);
	if (DistanceToTarget < NearestDistance)
	{
		NearestDistance = DistanceToTarget;
		NearestActor = TargetActor;
	}
}
```

`UBTFunctionLibrary`

```c++
#include "BehaviorTree/BTFunctionLibrary.h"

//设置Selector对应相应的值
UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, NearestDistance);
UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, NearestActor);
```

### 23.3.Behavior Tree中Decoration

通过给`Behavior Tree`中的`节点`添加`Decoration`来改变AI的行动

还可以运用`AIController`去改变`Blackborad`中属性的值

```c++
AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bHitReacting"), bHitReacting);
```

### 23.4.自定义Task

为了实现Attack功能引入的自定义Task

实现了寻找最近目标的附近随机点位

`GetRandomLocationInNavigableRadius`获取目标周围`Radius`内的随机`Location`

通过对Task的自己定义可以在ReciveExecuteAI中实现自己想要的功能

### 23.5.EQS(Environment Query System)

为了实现远程攻击手段

防止 当`Targat`在射程内前方由`障碍物`时，`Enemy`自动寻找最近的可以射击的地点

有相应的`EQSTestingPawn`蓝图模板进行检测

在EQS中可以设置想要生成的图案

通过添加测试来获得图案相应位置的分数来获取最佳点位

EQS效果展示图：

![image-20241129152812903](.\photos\EQS.png)

更改生成的图案

![image-20241129153246760](.\photos\EQS_1.png)

在BehaviorTree中运行`RunEQ` Task可以获取Score最高点的位置

## 24.Enemy Attack

### 24.1.Attack Ability

仍然是通过赋予`Enemy`相应的`Tag`来进行触发对应的`Ability`

```c++
//Attack
FGameplayTag Ability_Attack;

//.cpp

/*
* Attack
*/
AuraGameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(

	FName("Ability.Attack"),
	FString("Ability Attack Tag")
);

```

需要考虑到每种职业有不同的`Ability`和`Ability`的多样性

```c++
//CharacterClassInfo.h
//Data中添加
UPROPERTY(EditDefaultsOnly, Category = "Attack Default")
TArray<TSubclassOf<UGameplayAbility>> SetupAbilities;
```

初始化`Ability`

```c++
//获取Abilities数组
const FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterDefaultInfo(CharacterClass);
//在CombatInterface中可以获取Level
ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());

if (CombatInterface == nullptr) return;
//遍历去初始化每个Ability
for (TSubclassOf<UGameplayAbility> AbilityClassOf : DefaultInfo.SetupAbilities)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClassOf, CombatInterface->GetPlayerLevel());
	ASC->GiveAbility(AbilitySpec);
}
```

在蓝图中需要创建相应的`Ability`，并添加激活的`Tag`

由于通过`Task`来激活`Ability`故在`Attack Task`中去通过`AbilitySystemComponent`调用`Try Activate Abilities by Tag`

![image-20241130004528266](.\photos\ActivateAbility.png)

### 24.2.Attack Montage

动画的播放

蓝图选择函数

`PlayMontageAndWait`

`Attack Montage`播放前的转向`Motion warping`(在Montage 的RootMotion == true才能使用)



![image-20241130154229674](.\photos\MotionWarping.png)

需要在Montage中进行的设置 添加通知状态

![image-20241130154534495](.\photos\MotionWarping_1.png)

在应用`MotionTarget`需要调用`SetFacingTarget`函数其中

`SetFacingTarget`需要Location参数，在`TargetActor`中,故在`Ability`需要获取`TargetActor`参数，其中`TargetActor`在`Task`中可以通过`BlackboardKeySelector`获取，再通过`Interface`的函数实现参数的调用



具体实现方法：

```c++
#include "EnemyCharacter.h"
UPROPERTY(EditAnywhere, Category = "Combat")
TObjectPtr<AActor> CombatTarget;

virtual void SetCombatTarget_Implementation(AActor* CombatActor) override;
virtual AActor* GetCombatTarget_Implementation() override;


#include "EnemyInterface.h"
//在cpp中实现Implementation函数,蓝图中调用
UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
void SetCombatTarget(AActor* CombatActor);
UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
AActor* GetCombatTarget();
```

在`Task`中`SetCombatTarget`,在`Ability`中`GetCombatTarget`获取`Location`

### 24.3.Attack Evenet

结合通过`Montage`中的`Notify`

在通知中绑定相应的`Event Tag`用于触发`Event`

`Wait Gameplay Event`蓝图函数

来执行相应的`Event`

![image-20241201004633744](.\photos\WaitGameplayEvent.png)



当改函数是BlueprintNativeEvent需要在cpp中执行时需要执行类名::Execute_函数名()

```c++
const FVector SocketLocation = ICombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo());
```

### 24.4.Query Radius Actor

由于以前使用过的Apply Radial Damage with Falloff(范围衰减伤害)含有相应的功能我们可以模仿这个函数写出相关功能

```C++
void UAuraAbilitySystemLibrary::QueryActorsInRadius(const UObject* WorldContext, TArray<AActor*>& OutputActor, const TArray<AActor*>& IgnoreActors, const float Radius, const FVector SphereCenter)
{
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(IgnoreActors);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereCenter, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (const FOverlapResult& Overlap:Overlaps)
		{
			//检测Overlap角色是否继承了 CombatInterface类 且 是否死亡
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_isDie(Overlap.GetActor()))
			{
				OutputActor.AddUnique(Overlap.GetActor());
			}
		}
	}
}

```

### 24.5.Enemy Attack Damage

通过ApplyGameplayEffectSpecToTarget来使GE_Damage生效

```c++
void UAuraDamageGameplayAbility::CasusDamage(AActor* TargetActor)
{

	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (const auto& Damage : Damages)
	{
		const float ScaleDamage = Damage.Value.GetValueAtLevel(GetAbilityLevel());
		//EffectSpec绑定Tag和伤害
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Damage.Key, ScaleDamage);
	}

	//thisActor给TargetActor附加Effect
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
```

### 24.6.多种近战攻击方式

在敌人类型不同的情况，敌人的攻击方式会不同，导致原有的通过GetWeaponSocketLocation获取武器节点方式发动攻击的方法失效



解决方案:给与不同Montage不同的Tag来使得在GetWeaponSocketLocation时通过相应的Tag获取不同的节点



需要Montage和Tag结合，相应的Get函数

```c++

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
};


UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
TArray<FTaggedMontage> GetAttackMontages() const;
```

### 24.7.Ghoul Enemy

出现多个问题

1.Motion Warping没作用 Anim中没有设置 RootMotion

2.启动Ability时Tag和Montage出错,应该先将随机的Montage提升为变量在给后面的进行赋值

3.在攻击到敌人后攻击动画结束应该关闭PlayMontageAndWait 中的 Stop When Ability Ends

4.在Enemy攻击时打断敌人攻击敌人会卡住 

先endAbility，再GA_HitReact中CancelAbilitywithTag中添加Abilities.Attack消除Attack Tag

![image-20241204234249325](.\photos\AttackBug.png)

### 24.8.Ranger Attack

创建Ability 通过 ProjectileShell创建

创建相应的BP_Projectile

Attack的动画Montage



### 24.9.SlingShot 的动画优化



将Slingshot的绑定到Goblin的右手，在发射Rock时进行释放发射完成再绑定

在ABP中获取Owner Pawn再得到Get Mesh通过Mesh获取右手的Socket Transform并进行存储

![image-20241207190749013](.\photos\GetRightHandSocket.png)

再在ABP中调用Transform(Modify) Bone将Slingshot中相应的插槽绑定到相应的位置

![image-20241207190941381](.\photos\TransformBone.png)

需要ABP中添加一个bool来调控是否该松开通过Montage的通知来设置

![image-20241207191118669](.\photos\SetHolding.png)



### 24.10.Enemy Shaman

`Enemy`死亡时`BehaviorTree`未停止在BlackBoard中添加一个Key设置Enmey死亡否则会在死亡时不停止攻击



### 24.11.Sounds和ImpactEffect

在FAttackMontage中添加新参数

```c++
UNiagaraSystem* ImpactEffect;
USoundBase* ImpactSound;
```



### 24.11.在客户端运用Gameplay Cue

重载On Execute函数并使用其中的参数来获取外部数据

![image-20241213235637259](.\photos\GameplayCue.png)

在On Execute函数中去Spawn Niagara 和Play Sound



### 24.12.添加攻击轨迹(Trail)

在手上增加Socket，后在Montage中添加Trace TimedNiagaraEffect添加粒子效果并绑定在相应的Sokcet上

### 24.13.Demon Enmey

Demon分成2种 一种近战 一种远程

实现与其他Actor相同

### 24.14.召唤能力(Shaman Summon)

召唤敌人的生成位置函数

```c++
TArray<FVector> UAuraSummonGameplayAbility::GetRandomSummonLocation() 
{
    TArray<FVector> SpawnLocations;

    // 缓存Actor数据
    const AActor* AvatarActor = GetAvatarActorFromActorInfo();
    const FVector ActorLocation = AvatarActor->GetActorLocation();
    const FVector ActorForward = AvatarActor->GetActorForwardVector();

    // 预计算常量
    const float HalfAngle = MaxAngle * 0.5f;
    const FVector LeftDir = ActorForward.RotateAngleAxis(HalfAngle, FVector::UpVector);
    const FVector RightDir = ActorForward.RotateAngleAxis(-HalfAngle, FVector::UpVector);

    // 调试绘制
#if WITH_EDITOR
    UKismetSystemLibrary::DrawDebugLine(this, ActorLocation + MinRadius * LeftDir, ActorLocation + MaxRadius * LeftDir, FColor::Green, 5.f);
    UKismetSystemLibrary::DrawDebugLine(this, ActorLocation + MinRadius * RightDir, ActorLocation + MaxRadius * RightDir, FColor::Green, 5.f);
#endif

    TArray<FLocationConstraint> ExistingConstraints;
    const int32 MaxAttempts = NumToSummon * 10; // 防止无限循环
    int32 Attempts = 0;

    while (SpawnLocations.Num() < NumToSummon && Attempts++ < MaxAttempts) 
    {
        // 生成候选位置
        const float Radius = FMath::FRandRange(MinRadius, MaxRadius);
        const float Angle = FMath::FRandRange(-HalfAngle, HalfAngle);
        const FVector Direction = ActorForward.RotateAngleAxis(Angle, FVector::UpVector);
        const FVector CandidateLocation = ActorLocation + Radius * Direction;

        // 校验条件
        bool bValid = true;
        for (const FLocationConstraint& Constraint : ExistingConstraints) 
        {
            const FVector ExistingLocation = ActorLocation + Constraint.DistanceToOrigin * ActorForward.RotateAngleAxis(Constraint.GeneratedAngle, FVector::UpVector);
            const float Distance = FVector::Distance(CandidateLocation, ExistingLocation);
            const float AngularDiff =FMath::Abs(Constraint.GeneratedAngle - Angle)/* 实际角度差计算 */;

            if (Distance < SpawnMinSpace && AngularDiff < SpawnMinAngle) 
            {
                bValid = false;
                break;
            }
        }

		if (bValid)
		{
			ExistingConstraints.Add({ Radius, Angle });
			SpawnLocations.Add(CandidateLocation);
#if WITH_EDITOR
			UKismetSystemLibrary::DrawDebugSphere(this, CandidateLocation, 30.f, 12, FColor::Red, 5.f);
#endif
		}
    }

    return SpawnLocations;
}
```

进一步优化方法

1. **空间分区优化**
   - 使用网格（Grid）将召唤区域划分为单元格，每个单元格最多包含一个召唤点，可彻底避免距离校验。
2. **随机采样算法**
   - 采用泊松圆盘采样（Poisson Disk Sampling）算法生成位置，保证随机性的同时天然满足最小间距。
3. **异步生成**
   - 若`NumToSummon`极大（>1000），可将生成过程拆分到多帧进行，避免卡顿。

#### 24.14.2.召唤限制

SpawnActor后需要SpawnDefaultController

![image-20250203103514279](.\photos\image-20250203103514279.png)

复制单独的BehaviorTree,将Attack换成Elementalist Attack

在BTT_Attack中设置2中Abilities Tag的转换，通过MinionCount来控制

是否转换为Summon Tag

![image-20250203143222002](.\photos\image-20250203143222002.png)

对应召唤SpawnActor进行绑定事件Bind Event to Destroyed后，

绑定角色死亡进行事件不成功，原因：过早的结束了GA_Summon

解决方案:不在GA_Summon中结束这个Ability

导致,在后面Montage执行Notify时都会执行一遍Ability，

解决方案:可以在Ability中添加Cancel Tag(执行时会取消Tag的Ability)

#### 24.14.3.召唤出现动画

采用TimeLine进行实现，

在Actor出现时，

对Actor设置缩放来实现出现



## 25.关卡设计

### 25.1.地图设计

按照自己想法搭建Map

加入 Post Progress Volume(后期处理盒子)

​	Nav Mesh Bounds Volume(导航盒子)

对场景的Mesh添加相应的碰撞箱

调整场景的光亮和色彩

### 25.2.对于Texture超出预算的处理办法

1.对Texture材质进行降低(设置材质的分辨率支持多选一键设置)



2.设置Texture池大小 在DefaultEngine.ini中

```c++
[/Script/Engine.RenderSettings]
r.TextureStreaming=True
r.Streaming.PoolSize=1000
```

### 25.3.灯柱的设记和火焰闪烁(Flame Pillar Actor)

由于需要灯光进行闪烁

需要将Mesh 和 Niagara加入BP_Actor中并添加灯光

在蓝图中对Light进行TimeLine设置光照强度等信息来实现闪烁

可以多写几个TimeLine然后用不同的事件，在Switch中通过random触发

![image-20250211181051231](.\photos\image-20250211181051231.png)

### 25.4.摄像机视线遮挡(Fade Actor)

DitherTemporalAA来实现材质球的淡入和淡出



通过Interface来在Aura中实现对遮挡物的Fade Out和Fade In避免的需要调用FadeActor中的函数



Interface在蓝图实现需要在对应的Blueprint,类设置中

![image-20250211202817436](.\photos\image-20250211202817436.png)

在Interface中添加的函数可以在添加后用`Event+函数名`(Event Fade In)实现

由于需要在Aura中实现对FadeActor的FadeIn则需要 设置一个BoxCollision

当Begin Overlap时 `Does Object Implement Interface`实现



## 26.经验系统

### 26.1.重做OverlapWidget界面

![image-20250212175200498](.\photos\image-20250212175200498.png)

对HorizontalBox和VerticalBox的叠加使用

需要在Wbp_OverlapWidget中将此WBP进行SetWidgetController 然后在这WBP中对WBP_Health和WBP_Mana进行Set WidgetController

小的WBP_Spell射击可以参考WBP_Health

![image-20250212225602054](.\photos\image-20250212225602054.png)

需要初始化



### 26.2.xp栏 And Level up

xp栏位就是一个ProgressBar

#### 26.2.1.Xp的分级奖励

通过DataAsset进行设置

```c++
USTRUCT(BlueprintType)
struct FXpRewardAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XpRewardData")
	int32 RequiredXp =0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XpRewardData")
	int32 ShellPoint = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XpRewardData")
	int32 AttributePoint = 1;

};
```



#### 26.2.2.xp的获取C++

##### 26.2.2.1.击杀敌人时触发SendGameplayEventToActor 

通过在Attribute中判断是否死亡来触发

```c++
SendXpEvent(EffectParams);
```

在这个函数中我们需要获取被击杀敌人的奖励经验值并通过SendGameplayEventToActor传递给蓝图来传递经验值

```c++
void UAuraAttributeSet::SendXpEvent(FGameplayEffectParams& Props)
{
	// 当敌人死亡时触发的事件，获取经验值并发送给玩家
	// Target 是敌人，从敌人中获取经验值
	ICombatInterface* CombatInterface = CastChecked<ICombatInterface>(Props.TargetAvatarActor);
	if (CombatInterface)
	{
		// 获取敌人的等级
		const float TargetLevel = CombatInterface->GetPlayerLevel();
		// 获取敌人的角色类型
		const ECharacterClass TargetCharacterClass = CombatInterface->Execute_GetCharacterClass(Props.SourceCharacter);

		// 获取游戏标签
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		// 设置事件标签为获取经验值
		Payload.EventTag = GameplayTags.Attribute_Meta_IncomingXp;
		// 计算并  设置事件的经验值大小
		Payload.EventMagnitude = UAuraAbilitySystemLibrary::GetRewardXpForClassAndLevel(Props.TargetAvatarActor, TargetLevel, TargetCharacterClass);

		// 发送游戏事件给玩家，传递经验值
UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, Payload.EventTag, Payload);
	}
}
```

同时我们`UAuraAbilitySystemLibrary::GetRewardXpForClassAndLevel`写了一个通过CharacterClass和Level获取Xp的函数，原理就是我们在`CharacterInfo`中添加一个`FScalableFloat` 的经验曲线，通过Level和CharacterClass获取其中一条曲线的经验值

##### 26.2.2.2.发送给Ability(专门用来监听Xp的)发送Tag 和 XPReward信息

现在我们在蓝图中写好的Ability收到了C++中发出的信号SendGameplayEventToActor

通过Tag来发送和接收的，但是我们需要提前将这个Ability Apply在玩家身上才会生效所有我们在AbilitySystemComponent中提前GiveAbilityAndActivateOnce

```c++
void UAuraAbilitySystemComponent::GrantGameplayPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClassOf : GameplayAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClassOf, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}
```

这样我们可以收到C++发出的信号并接受

![image-20250224174058184](.\photos\image-20250224174058184.png)



且这个Ability需要设置为Instancing Policy设置为 Instanced Per Actor 和 

Net Execution Policy 设置为Server Only 因为我们只要初始化一次且经验的计算只在Server上

##### 26.2.2.3.触发Effect去增加Attribute IncomingXp

这是会触发GameplayEffect 来将xp数据传输到我们Attribute IncomingXp上

我们需要添加一个新的Attribute存储临时经验 还需要一个Tag来传递信号即可

在C++中我们检测到有Attribute是IncomingXp

```c++
if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
{
	const int32 LocalIncomingXp = GetIncomingXp();
	SetIncomingXp(0);
	//设置PlayerState中的经验值
	if (EffectParams.SourceCharacter->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToXp(EffectParams.SourceCharacter, LocalIncomingXp);
	}
}
```

##### 26.2.2.4.把IncomingXp转化为PlayerState的Xp

现在我们是IncomingXp我们需要转换到PlayerState中故

```c++
void AAuraCharacter::AddToXp_Implementation(int32 Xp)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		AuraState->AddXp(Xp);
	}
}
```

##### 26.2.2.5.触发Xp的OnRep，并传递Xp给Widget计算Percent发送给蓝图

而且我们在PlayerState将Xp设置了Replication

所以会触发OnRep_Xp函数

```c++
void AAuraPlayerState::OnRep_Xp(int32 OldXp)
{
	//TOOD: Level up
	OnXpChangedDelegate.Broadcast(Xp);
}
```

我们只需要将Xp传递给Widget

OnXpChangedDelegate委托绑定了OnXpChange

```c++
void UOverlapWidgetController::OnXpChange(int32 NewXp)
{
	//获取现在等级
	const AAuraPlayerState* AuraState = CastChecked<AAuraPlayerState>(PlayerState);

	UXpRewardData* XpRewardData = AuraState->XpData;
	checkf(XpRewardData, TEXT("Please Put XpData In BP_AuraPlayerState"));

	float Percent;
	const int32 Level = XpRewardData->FindLevelAtXp(NewXp);

	//如果等级已经到最高了
	if (Level == XpRewardData->XpRewardDataArray.Num())
	{
		Percent = 1.f;
	}
	else
	{
		const int32 CurrentLevelXp = XpRewardData->XpRewardDataArray[Level - 1].RequiredXp;
		const int32 RequiredLevelXp = XpRewardData->XpRewardDataArray[Level].RequiredXp;
		const int32 CurrentXp = NewXp - CurrentLevelXp;
		const int32 RequiredXp = RequiredLevelXp - CurrentLevelXp;
		Percent = static_cast<float>(CurrentXp) / static_cast<float>(RequiredXp);
	}
	OnXpPercentChangeSingnature.Broadcast(Percent);
}

```

然后通过委托OnXpPercentChangeSingnature将计算好的Percent传递给蓝图即可

在蓝图中先进行Set Widget Controller再将Event掉用并设置Percent就好了

##### 

### 26.3.定义自定义log(UE_Log中的类别)

#### 26.3.1. 定义 Log Category

在全局范围内定义一个 Log Category。通常，你会在一个全局头文件（例如 `MyProject.h`）中定义它。

```c++
// MyProject.h
#pragma once

#include "CoreMinimal.h"

// 定义自定义 Log Category
DECLARE_LOG_CATEGORY_EXTERN(LogMyGame, Log, All);
```

- `LogMyGame` 是你自定义的 Log Category 名称。
- `Log` 表示默认的日志级别。
- `All` 表示所有日志输出级别（Verbose、Warning、Error 等）都启用。

------

#### 26.3.2. 实现 Log Category

在对应的 `.cpp` 文件中实现 Log Category。

```c++
// MyProject.cpp
#include "MyProject.h"

// 实现 Log Category
DEFINE_LOG_CATEGORY(LogMyGame);
```



### 26.4.数据的存储与对AbilityInfo的Widget运用

#### 26.4.1.数据的存储

生成一个DataAsset的C++类，写出相应的结构体

```c++
USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BackGroundImage = nullptr;
};


UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityData")
TArray<FAuraAbilityInfo> AbilityInfomation;
```

填写相应数据即可

#### 26.4.2.对Data的调用

由于需要对Data数据在Widget中进行调用,且由于是显示Ui的数据每次更改后都需要变动,所有我们选择使用委托进行传递数据

我们先在`AuraAbilitySystemComponent`中声明一个委托

因为我们需要在`AuraAbilitySystemComponent`中进行调用

因为在这个类中我们`GrantGameplayAbilities`赋予Ability相关的InputTag

```c++
DECLARE_MULTICAST_DELEGATE_OneParam(FDataInfoDelegate, UAuraAbilitySystemComponent*)

FDataInfoDelegate AbilityDeleagte;
```

在OverlapWidgetController进行绑定



因为需要反复调用所有我们写一个函数委托和这个函数进行绑定

```c++
//是否绑定过
if (AuraASC->bStartAbilityDeleagte)
{
	OnInitAbilityInfo(AuraASC);
}
else
{
	//Ability绑定委托
	AuraASC->AbilityDeleagte.AddUObject(this, &UOverlapWidgetController::OnInitAbilityInfo);
}
```

先进行委托的绑定，之后在`AuraAbilitySystemComponent`类中`GrantGameplayAbilities`执行这个绑定的函数

OnInitAbilityInfo

```c++
void UOverlapWidgetController::OnInitAbilityInfo(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	if (!AuraAbilitySystemComponent->bStartAbilityDeleagte) return;

	//TOOD: 需要对Data中的InputTag进行设置,需要激活的能力，

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo info = AbilityInfo.Get()->FindAbilityInfoByAbilityTag(AuraAbilitySystemComponent->GetAbilityTagByAbilitySpec(AbilitySpec));
			info.InputTag = AuraAbilitySystemComponent->GetInputTagByAbilitySpec(AbilitySpec);
			OnAbilityInfoSingnature.Broadcast(info);
		});
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);

}
```

在这个函数中我们再绑定一个委托，主要用于设置DataInfo的InputTag，从AuraAbility中进行获取

并且将Info数据发送到蓝图中，我们需要一个动态多播委托

```c++
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSingnature, const FAuraAbilityInfo&, Info);
```

将数据发送到蓝图并方便调用

绑定完成我们需要对所有玩家拥有的Ability进行调用这个绑定的委托

```c++
void UAuraAbilitySystemComponent::ForEachAbility(FForEachAbility Delegate)
{
	// 使用作用域锁（Scoped Lock）锁定当前 Ability 列表，确保遍历过程中列表不会被修改。
	// 这是为了避免在迭代过程中因其他线程或逻辑修改 Ability 列表导致的数据竞争或不一致性。
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	// 遍历所有可激活的 Ability（通过 GetActivatableAbilities 获取列表）
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 执行委托，将当前 AbilitySpec 作为参数传递。
		// ExecuteIfBound 返回 `false` 表示委托未绑定或执行失败。
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			// 记录错误日志：输出当前函数名（__FUNCTION__）和委托执行失败的信息。
			// 注意：UE_LOG 的格式化字符串中，%hs 表示 ANSI 字符串（__FUNCTION__ 是编译器宏，返回当前函数名）。
			UE_LOG(AuraLog, Error, TEXT("Failed to execute delegate in function [%hs]"), __FUNCTION__);
		}
	}
}
```

即可完成对DataInfo数据的填充完成

#### 26.4.3.蓝图中数据的调用

首先我们需要将需要调用OverlapWidgetController内部函数的WBP进行`SetWidgetController`

![image-20250219202651092](.\photos\image-20250219202651092.png)

并且我们在C++写了一个动态多播进行传递数据，我们需要在WBP中进行调用

![image-20250219202927649](.\photos\image-20250219202927649.png)

这样我们就可以在蓝图中获得C++的数据了接下来就是Tag的匹配和设置图案了

![image-20250219203029398](.\photos\image-20250219203029398.png)

#### 26.4.4.客户端的应用

由于GrantGameplayAbilities只在服务端调用所以我们需要

```c++
virtual void OnRep_ActivateAbilities();
```

在OnRep_ActivateAbilities函数中调用GrantGameplayAbilities一样的即可

### 26.4.Ability Cost And Cooldown

在Ability中有相应的位置可以放置Cost和Cooldow Effect

![image-20250220173100675](.\photos\image-20250220173100675.png)

使用后需要在蓝图中调用`Commit Ability`函数

Cooldown同理需要一个GameplayEffect 但是Cooldown需要一个对应的Tag，原理是当执行Ability时被赋予了Tag

### 26.5.UI图标冷却和显示Cooldown数值(运用了Async 异步)

#### 26.5.1.Async类的创建

由于要创建异步函数我们我们先创建`UBlueprintAsyncActionBase`类

由于是异步函数需要 CooldownStart 和 CooldownEnd 2个委托且要在蓝图中实现所有实现一个动态多播委托

```c++
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownAsyncTask, float, RemainingTime);
```

和一个在蓝图中调用的异步函数 需要是static

```c++
/**
 * 等待冷却时间的开始。
 *
 * @param AbilitySystemComponent 要监控的能力系统组件。
 * @param InCooldownTag 表示冷却时间的游戏标签。
 * @return 创建的 UCooldownAsyncTask 实例的指针。
 */
UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
static UCooldownAsyncTask* WaitforCooldownStart(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InCooldownTag);
```

且是静态函数故类中需要变量

```c++
/** 被监控的能力系统组件。 */
UPROPERTY()
UAbilitySystemComponent* ASC;

/** 表示冷却时间的游戏标签。 */
FGameplayTag CooldownTag;
```

对函数的实现

在WaitforCooldownStart主要目的绑定CooldownStart和CooldownEnd委托

```c++
UCooldownAsyncTask* UCooldownAsyncTask::WaitforCooldownStart(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InCooldownTag)
{
	UCooldownAsyncTask* CooldownAsyncTask = NewObject<UCooldownAsyncTask>();
	if (!AbilitySystemComponent || !InCooldownTag.IsValid())
	{
		CooldownAsyncTask->AsyncTaskEnd();
		return nullptr;
	}

	CooldownAsyncTask->ASC = AbilitySystemComponent;
	CooldownAsyncTask->CooldownTag = InCooldownTag;

	// 绑定到能力激活时触发的委托（CooldownStart）
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(CooldownAsyncTask, &UCooldownAsyncTask::GameplayEffectApplyToSelf);

	// 绑定到标签增加或移除时触发的委托（CooldownEnd）
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(CooldownAsyncTask, &UCooldownAsyncTask::GameplayTagEvent);

	return CooldownAsyncTask;
}
```

替新委托写绑定函数在绑定函数中对OnCooldownStart 和 OnCooldownEnd进行Broadcast

```c++
/**
 * 当游戏效果应用到自身时调用。
 *
 * @param AbilitySystemComponent 能力系统组件。
 * @param GameSpec 游戏效果规格。
 * @param GameplayHandle 活动游戏效果句柄。
 */
void GameplayEffectApplyToSelf(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameSpec, FActiveGameplayEffectHandle GameplayHandle);

/**
 * 当游戏标签事件发生时调用。
 *
 * @param InCooldownTag 游戏标签。
 * @param NewCount 标签的新计数。
 */
void GameplayTagEvent(const FGameplayTag InCooldownTag, int32 NewCount);
```



查询Effect时间的函数

 AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);

```c++

void UCooldownAsyncTask::GameplayEffectApplyToSelf(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameSpec, FActiveGameplayEffectHandle GameplayHandle)
{
	FGameplayTagContainer AssetTags;
	GameSpec.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantTags;
	GameSpec.GetAllGrantedTags(GrantTags);

	// 检查游戏效果是否为冷却效果
	if (AssetTags.HasTagExact(CooldownTag) || GrantTags.HasTagExact(CooldownTag))
	{
		// 获取剩余冷却时间
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> CooldownTimes = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);

		if (CooldownTimes.Num() > 0)
		{
			// 获取最大冷却时间
			float MaxCooldownTime = CooldownTimes[0];
			for (int i = 0; i < CooldownTimes.Num(); i++)
			{
				MaxCooldownTime = FMath::Max(MaxCooldownTime, CooldownTimes[i]);
			}

			// 广播冷却时间开始事件
			OnCooldownStart.Broadcast(MaxCooldownTime);
		}
	}
}

void UCooldownAsyncTask::GameplayTagEvent(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		// 广播冷却时间结束事件
		OnCooldownEnd.Broadcast(0.f);
	}
}
```



#### 26.5.2.蓝图实现

UCLASS(BlueprintType,meta = (ExposedAsyncProxy = "AsyncTask"))

将AsyncTask暴露给蓝图节点

放置AsyncTask多次声明

![image-20250221201730065](.\photos\image-20250221201730065.png)

设置透明和`Set Timer By Event`一个计时器

### 26.6.Level up

#### 26.6.1.升级奖励

首先在获取经验处判断是否升级

```c++
if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
{
	const int32 LocalIncomingXp = GetIncomingXp();
	SetIncomingXp(0);
	//设置PlayerState中的经验值
	if (EffectParams.SourceCharacter->Implements<UPlayerInterface>()&&EffectParams.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 Level = ICombatInterface::Execute_GetPlayerLevel(EffectParams.SourceCharacter);
		const int32 CurrentXp = IPlayerInterface::Execute_GetPlayerXp(EffectParams.SourceCharacter)+ LocalIncomingXp;

		const int32 CurrentLevel = IPlayerInterface::Execute_FindLevelForXp(EffectParams.SourceCharacter, CurrentXp);

		int32 UpLevel = CurrentLevel - Level;
		if (UpLevel > 0)
		{

			int32 AttributePointsReward = 0;
			int32 ShellPointsReward = 0;

			for (int32 i = 0; i < UpLevel; i++)
			{
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointReward(EffectParams.SourceCharacter, Level + i);
				ShellPointsReward += IPlayerInterface::Execute_GetShellPointReward(EffectParams.SourceCharacter, Level + i);
			}
			
			IPlayerInterface::Execute_AddToAttributePoint(EffectParams.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToShellPoint(EffectParams.SourceCharacter, ShellPointsReward);

			SetHealth(GetMaxHealth());
			SetMana(GetMaxMana());

			IPlayerInterface::Execute_LevelUp(EffectParams.SourceCharacter, UpLevel);
		}
		IPlayerInterface::Execute_AddToXp(EffectParams.SourceCharacter, LocalIncomingXp);
	}
}
```

判断升级同时 将升级奖励给出，并设置满状态即可

#### 26.6.2.显示等级和特效显示

特效播放在Server端

需要Multicast RPC

且特效的旋转位置有误需要调整

```c++
void AAuraCharacter::MulticastLevelupEffect_Implementation()
{
	const  FVector EffectLocation = LevelupEffect->GetComponentLocation();
	const FVector CameraLocation = FollowCamera->GetComponentLocation();
	const FRotator Rotation = (CameraLocation - EffectLocation).Rotation();
	LevelupEffect->SetWorldRotation(Rotation);
	LevelupEffect->Activate(true);
}

```



关于等级显示我们在PlayerState中有一个升级的委托，故每次等级提示后会执行绑定函数

我们在Overlap类中绑定相关函数，并写一个可以发送Level到蓝图的动态多播委托即可

```c++
void UOverlapWidgetController::OnLevelChange(int32 NewLevel)
{
	OnLevelChangeDelegate.Broadcast(NewLevel);
}
```



#### 26.6.3.浮动文字提示

浮动文字就像Message

需要用的时候，即升级后Create Widget 之后Add In Viewport 即可

![image-20250226172338526](.\photos\image-20250226172338526.png)

Play Sound 和 Play Animation在Level Message构造完成后使用

#### 26.6.4.升级后血量重置不完全

由于在升级在PerAttributeChange中执行的

```c++
SetHealth(GetMaxHealth());
```

但升级后并不会立刻执行Health新值的计算
会在执行SetHealth之后执行导致升级后血量并未回满

##### 解决方法：

在PostAttributeChange中执行SetHealth即可



## 27.技能菜单

### 27.1.技能菜单的Show 和 Hide菜单制作

#### 27.1.1. Show

菜单的出现需要一个按钮来控制菜单的出现

按钮的函数实现:

1.首先通过触发`按钮的Click事件`进行，来触发另外一个事件，这个事件是为了实现`按钮功能的事件`

2.设置按钮不可用`Set Enable`

3.创建需要添加的菜单 `Create Widget`

4.添加到窗口 `Add To ViewPort`
5.设置位置 `Set Position`

#### 27.1.2. Hide

菜单的消失则需要在菜单中的关闭进行实现
1.首先还是这个按钮的Click事件,需要在这个菜单构建`ConstructEvent`时进行申请事件

2.功能调用`Remove from Parent`

3.菜单调用`Event Destruct`事件去调用一个新事件(为了恢复外面菜单的使用) 注意:这个事件需要在这个菜单的事件分发器中进行声明,实际调用需要在主菜单中实现

4.主菜单进行实现事件`Set Enable`

#### 27.1.2.分离玩家操作和UI的功能

通过
Set Input Mode UI Only 不能操控玩家只能动鼠标点一些按钮
Set Input Mode Game And UI 既有按钮又可以WASD等控制

Set Input Mode Game Only 只有WASD等控制

来实现

### 27.2.菜单功能的实现

#### 27.2.1.技能装配栏的信息显示

通过AbilityInfo中的信息来设置装配栏



所有的菜单Controller都是通过HUD对外访问的

1.在HUD中添加MenuController和Class
2.写GetWidgetController进行设置几大基础属性

3.AbilityInfo是Delegate需要Broadcast在InitBroadcast中调用
4.InitBroadcast也需要在蓝图中调用才正式应用AbilityInfoDelegate
5.在相应位置使用AbilityInfoDelegate这个Event

#### 27.2.2.对于主动技能的解锁

现在情况是
AbilityInfo的RequiredLevel需要发挥作用
当等级变化时判断是否满足升级条件并修改SpellState,

修改State使得技能解锁
需要对State进行OnRep操作

#### 27.2.3.技能点的显示

由于在AuraPlayerState中添加过对ShellPoint的Delegate和Broadcast

所以在SpellMenuWidgetController中对ShellPointChangeDelegate进行AddLambda
lambda表达式中只需要添加一个新的Delegate的Broadecast
这个新Delegate需要在蓝图中Assign来实现对Text的修改

#### 27.2.4.选择技能球



#### 27.2.5.按钮的Enable实现





