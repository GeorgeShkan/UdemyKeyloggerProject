param ( [String]$Att, 
        [String]$Sub, 
        [String]$body )

Function Send-Email
{
    Param(
        [Parameter(`
            Mandatory = $true) ]
        
        [String]$To,
        [Parameter(`
            Mandatory = $true)]
            
        [String]$From,
        [Parameter(`
            Mandatory = $true)]
            
        [String]$Password,
        [Parameter(`
            Mandatory = $true)]

        [String]$Body,
        [Parameter(`
            Mandatory = $true)]

        [String]$Attachment
    )


    try 
    {
        $Msg = New-Object System.Net.Mail.MailMessage($To,$From, $Subject, $Body)
        $Srv = "smtp.gmail.com"
        if($Attachment -ne $null)
        {
            try
            {
                $attachment = Attachment -split ("\:\:");

                ForEach($val in $attachment)
                {
                    $attch = New-Object System.Net.Mail.Attachment($val)
                    $Msg.Attachments.Add($attch)
                }
            }
            catch{
                exit 2;
            }

            $Client = New-Object = Net.Mail.SmtpClient($Srv, 587)
            $Client.EnableSsl = $true
            $Client.Credentials = New-Object System.Net.NetworkCredential($From.Split("@")[0], $Password);
            $Client.Send($Msg)
            remove-variable -Name Client
            remove-variable -Name Password
            exit 7;
        }
    }

    catch
        {
            exit 3;
        }
}

try
{
    Send-Email
        -Attachment = $Att
        -To "recipient address"
        -Body $Body
        -Subject $Sub
        -password "senders password"
        -From "sender address"
}
catch 
{
    exit 4;
}